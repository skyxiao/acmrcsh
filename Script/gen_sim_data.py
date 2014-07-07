import MySQLdb
from datetime import *
import time
import random
 

conn=MySQLdb.connect(host='localhost',user='acm',passwd='acm',port=3306)
conn.select_db('dry_etch')
cur=conn.cursor()
t = datetime.now()

for i in range(0, 2000):
	cur.execute("call record_data('%s', '%s', %f)"%(t, 'HF', random.randint(0, 2000)))
	cur.execute("call record_data('%s', '%s', %f)"%(t, 'EtOH', random.randint(0, 700)))
	cur.execute("call record_data('%s', '%s', %f)"%(t, 'N2', random.randint(0, 5000)))
	cur.execute("call record_data('%s', '%s', %f)"%(t, 'Pressure', random.randint(1, 50)))
	cur.execute("call record_data('%s', '%s', %f)"%(t, 'Lid', random.randint(30, 40)))
	cur.execute("call record_data('%s', '%s', %f)"%(t, 'Chuck', random.randint(30, 40)))
	cur.execute("call record_data('%s', '%s', %f)"%(t, 'Body', random.randint(30, 40)))
	cur.execute("call record_data('%s', '%s', %f)"%(t, 'Tank', random.randint(30, 40)))
	t += timedelta(0, 1, 0)
	conn.commit()

cur.close()
conn.close()