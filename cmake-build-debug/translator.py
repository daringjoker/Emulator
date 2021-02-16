import subprocess
from collections import namedtuple
import json
import os
from subprocess import PIPE

registers=namedtuple("registers",["A","B","C","D","E","H","L","SP","PC","Flags"])
emu_path=os.path.join("C:\\Users\\DaringJoker\\Documents\\programming\\emulator\\cmake-build-debug","emulator.exe")

class Emulator:
	def __init__(self,program):
		command=emu_path+" -j "+program;
		self.proc=subprocess.Popen(command,stdin=PIPE,stdout=PIPE,close_fds=True)
		self.In=self.proc.stdin
		self.Out=self.proc.stdout
		self._readOutput()
		self.refresh_regs()


	def __del__(self):
		self.close()


	def _readOutput(self):
		output_buffer=b""
		while not output_buffer.endswith(b">> "):
			output_buffer+=self.proc.stdout.read(1)
		return output_buffer.decode("utf8")


	def _sendCommand(self,command):
		self.In.write(bytes(command,"utf8")+b"\r\n")
		self.In.flush()


	def issueCommand(self,command):
		self._sendCommand(command)
		return self._readOutput()


	def refresh_regs(self):
		out=self.issueCommand("r")
		regs="["+(out.split("[")[-1].split("]")[0])+"]"
		regs=json.loads(regs)
		self.registers=registers(*regs)


	def get_mem(self):
		out=self.issueCommand("m")
		mem="{"+(out.split("{")[-1].split("}")[0])+"}"
		return json.loads(mem)


	def get_ports(self):
		out=self.issueCommand("p")
		ports="{"+(out.split("{")[-1].split("}")[0])+"}"
		return json.loads(ports)


	def set_memory(self,address,value):
		command="wm "+str(address)+" "+str(hex(value)[2:])+"H"
		self.issueCommand(command)


	def set_port(self,address,value):
		command="wp "+str(address)+" "+str(hex(value)[2:])+"H"
		self.issueCommand(command)


	def single_step(self):
		self.issueCommand("n")
		self.refresh_regs()


	def continue_execution(self):
		self.issueCommand("c")
		self.refresh_regs()


	def close(self):
		if(self.proc.poll() is None):			
			self._sendCommand("q")
			self.proc.kill()

if __name__ == '__main__':
	emu=Emulator("doit.asm")
	print(emu.registers)
	emu.single_step()
	print(emu.registers)