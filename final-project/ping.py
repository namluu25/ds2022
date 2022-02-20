import socket

def isValidIP(ip):
    ad = ip.split(":")
    if len(ad) > 2:
        return False
    try:
        socket.inet_aton(ad[0])
    except socket.error:
        return False
    if len(ad) > 1:
        if len(ad[1])>0:
            try:
                _ = int(ad[1])
                return True
            except:
                return False
    return False
