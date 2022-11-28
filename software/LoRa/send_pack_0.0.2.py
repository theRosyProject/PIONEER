# Federico Dallo
# 4 may 2021

# main program
# 1- (sender) read message and make md5 checksum
# 2- (sender) split message into submessages and count how many
# 3- (sender) send submessages, count and md5
# 4- (receiver) build back message and check consistency
# 5- (receiver) aknowledge?!
# 6- (sender) receive akn and delete message

import zlib, sys, hashlib, difflib
import subprocess, time, random

fname = "msg"
try:
    fh = open(fname)
except:
    print("Can't open file.")
    quit()

for line in fh:
    print("\nThe message to be sent and its size (bytes):")
    a = line.rstrip() #remove \n char
    print(a, "(" + str(sys.getsizeof(a)) + ")")
    print("\nThis is the md5sum and its size (bytes):")
    ahash = hashlib.md5(a.encode('UTF-8')).hexdigest()
    print(ahash, "(" + str(sys.getsizeof(ahash)) + ")")

a_split = a.split()
split_size = len(a_split)
print(a_split, str(split_size), str(ahash))

# build message to be sent:
a_split.insert(0,str(len(a_split)))
a_split.insert(0,str(ahash))
# insert start message and random number
tmp_rand = random.randint(100,200)
a_split.insert(0,"start_msg" + str(tmp_rand))
a_split.append("end_msg" + str(tmp_rand))
print(a_split)

f_pk_name = "packet"

for i in a_split:
    tmp_f = open(f_pk_name, 'w')
    tmp_f.write(i)
    tmp_f.close()
    subprocess.call("/home/pi/cronPrograms/LoRa/sendPacket.sh")
    #time.sleep(1)
    time.sleep(0.1)

open(f_pk_name, "w").close()
