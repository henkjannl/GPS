class Buffers():

	def __init__(self):
		self.bufferCurrentlyWrittenTo = None
		self.bufferCurrentlyReadFrom  = None

		self.mostRecentStack = [1,2,3]

		self.content={ 1:'Not yet initialized',
		               2:'Not yet initialized',
		               3:'Not yet initialized'}
		

	def writeNewFrame(self, NewContent):

		# Take the number of the oldest frame on the most recent stack
		if (self.mostRecentStack[-1]!=self.bufferCurrentlyReadFrom):
			# The reading routine is not accessing the oldest frame
			# We can write to the oldest frame
			self.bufferCurrentlyWrittenTo=self.mostRecentStack[-1]
			self.content[self.bufferCurrentlyWrittenTo]=NewContent
		else:
			# The reading routine is currently accessing the oldest frame
			# We can write to the oldest-but-one frame
			self.bufferCurrentlyWrittenTo=self.mostRecentStack[-2]
			self.content[self.bufferCurrentlyWrittenTo]=NewContent

		self.mostRecentStack.insert(0,self.bufferCurrentlyWrittenTo)
		
	def readNewFrame(self):

		if (self.mostRecentStack[0] != self.bufferCurrentlyWrittenTo):
			# The most recent frame is no longer written to
			
