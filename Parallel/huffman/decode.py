#Jake Shankman
#Period 5
#9/5/2012
#Decode


    
code = open('decode.txt','r').read().split()
numItem = code.pop(0)
codec = {}
i = 0
while i < int(numItem):
  string = code.pop(0)
  codec[string[1:]] = string[0]
  i += 1
  
print codec
message = code.pop()
string = ''
temp = ''
i = 0
while i < len(message):
  temp += message[i]
  if temp in codec.keys():
    string += codec[temp]
    temp = ''
  i += 1  
print string
  