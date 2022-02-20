from concurrent import futures
import argparse
import grpc
import time

import ping
import kv_pb2, kv_pb2_grpc

args = []
store = {}
peers = []


def parseArgs():
    global args
    parser = argparse.ArgumentParser(description="Key-value store server.")
    parser.add_argument("peers", nargs="*", metavar="PEER",
                        help="Set slave server IP address (IPv4 only!).")
    parser.add_argument("--ip", default="127.0.0.1:4000",
                        help="Set server IP address (IPv4 only!).")
    args = parser.parse_args()

    if ping.isValidIP(args.ip) == False:
        print("not a valid IP address: '%s'" % args.ip)

    for peerIP in args.peers:
        if ping.isValidIP(peerIP) == False:
            print("not a valid peer IP address: '%s'" % peerIP)
        if peerIP != args.ip:
            if (peerIP in peers) == False:
                peers.append(peerIP)
                createSlave(peerIP)

    return args


class Storer(kv_pb2_grpc.ClientServicer):
    def Get(self, request, context):
        key = request.key
        if key in store:
            print("received GET request for key '{0:s}': value = '{1:s}'".format(
                key, store[key]))
            return kv_pb2.GetReply(value=store[key], defined=True)
        else:
            print("received GET request for key '%s': value = undefined" % key)
            return kv_pb2.GetReply(value=None, defined=False)

    def Set(self, request, context):
        key = request.key
        value = request.value
        broadcast = request.broadcast
        store[key] = value
        if broadcast:
            print(
                "received SET request for key '{0:s}': new value = '{1:s}'".format(key, value))
            updatePeers(key, value)
        else:
            print(
                "received peer update for key '{0:s}': new value = '{1:s}'".format(key, value))
        return kv_pb2.SetReply(value=value)

    def List(self, request, context):
        print("received LIST request")
        return kv_pb2.StoreReply(store=store)

    def RegisterWithPeer(self, request, context):
        peerIP = request.ip
        print("received new peer registration: %s" % peerIP)
        if ping.isValidIP(peerIP):
            if (peerIP in peers) == False:
                peers.append(peerIP)
        return kv_pb2.StoreReply(store=store)


def createSlave(peerIP):
    global store
    with grpc.insecure_channel(peerIP) as channel:
        stub = kv_pb2_grpc.ClientStub(channel)
        print("registering with peer %s..." % peerIP)
        response = stub.RegisterWithPeer(kv_pb2.IP(ip=args.ip))
        for key in response.store:
            store[key] = response.store[key]


def updatePeers(key, value):
    for peerIP in peers:
        print("updating peer '{0:s}': '${1:s}' = '${2:s}'".format(
            peerIP, key, value))
        with grpc.insecure_channel(peerIP) as channel:
            stub = kv_pb2_grpc.ClientStub(channel)
            stub.Set(kv_pb2.SetKey(key=key, value=value, broadcast=False))


def serve(ip):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    kv_pb2_grpc.add_ClientServicer_to_server(Storer(), server)
    server.add_insecure_port(ip)
    print("Listening on %s..." % ip)
    server.start()
    try:
        while True:
            time.sleep(60 * 60 * 24) # 24h in second
    except KeyboardInterrupt:
        server.stop(0)

if __name__ == "__main__":
    args = parseArgs()
    serve(args.ip)
