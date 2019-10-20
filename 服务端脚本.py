# coding:utf-8

import socket
import psutil

s=socket.socket()
s.bind(('127.0.0.1',9999))
s.listen(50)

white_list = set()

print('start listen...')
while True:
    conn,addr = s.accept()
    print(addr)
    rec = conn.recv(2048).replace(b'\x00', b'').split(b'\n')
    if rec[0] == b'L':
        if rec[1] == b'TEST_USER' and rec[2] == b'12345678':
            white_list.add(addr[0])
            conn.send(b'\x01')
            print('login succ')
        else:
            conn.send(b'\x00')
            print('login err')

    elif rec[0] == b'G':
        if addr[0] in white_list:
            conn.send(b'\r\nCPU-per: %d%%\r\nmem-per:%d%%\r\nnet-send:%d kb\r\nnet-recv:%d kb' % (int(psutil.cpu_percent(1)),int(psutil.virtual_memory()[2]),int(psutil.net_io_counters()[0]/8/1024/1024),int(psutil.net_io_counters()[1]/8/1024/1024)))
            print('return G')
            print(rec)
        else:
            conn.send(b'\x00')
            print('not in whitelist')

    else:
        print('other')
        print(rec)

