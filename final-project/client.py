from __future__ import print_function
import ping
import kv_pb2, kv_pb2_grpc
import grpc


def Get(ip, key):
    with grpc.insecure_channel(ip) as channel:
        stub = kv_pb2_grpc.ClientStub(channel)
        response = stub.Get(kv_pb2.GetKey(key=key))
        if response.defined:
            print("'%s'='%s'" % (key, response.value))
        else:
            print("'%s': undefined" % key)


def Set(ip, key, value):
    with grpc.insecure_channel(ip) as channel:
        stub = kv_pb2_grpc.ClientStub(channel)
        stub.Set(kv_pb2.SetKey(key=key, value=value, broadcast=True))


def List(ip):
    with grpc.insecure_channel(ip) as channel:
        stub = kv_pb2_grpc.ClientStub(channel)
        response = stub.List(kv_pb2.Void())
        print("Key-value pairs defined on %s:" % ip)
        for key in response.store:
            print("  - '{0:s}'='{1:s}'".format(key, response.store[key]))


def run():
    while(True):
        ip = input('Enter IP: ')
        if ping.isValidIP(ip):
            while(True):
                    #List(ip)
                    choice = int(input('0. Set    1. Get    2. List\n'))
                    if choice == 2:
                        List(ip)
                    elif choice == 1 or choice == 0:
                        key = input('Please enter Key: ')

                        if choice == 1:
                            Get(ip, key)
                        if choice == 0:
                            kv = input('Enter Value: ')
                            Set(ip, key, kv)
                
                    if KeyboardInterrupt or choice == 3:
                        break
        else:
            print('Not a valid IP!')


if __name__ == '__main__':
    run()
