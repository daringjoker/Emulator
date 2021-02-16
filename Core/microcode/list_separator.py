
import re
import os
for _,_,files in os.walk(os.getcwd()):
	for file in files:
		print(file.split(".")[0]+"=[",end="")
		with open(file) as infile:
			data=infile.read()
			lines=data.split("\n")
			for line in lines:
				matches=re.match(".*?::(\\w+)\\(.*?Decoded_Instr.*?\\).*?",line)
				if(matches):
					print("'"+matches.group(1)+"',",end="")
		print("]")