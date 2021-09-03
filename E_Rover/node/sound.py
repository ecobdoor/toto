from gtts import gTTS
import sys
print("\n======================================")
# total arguments
n = len(sys.argv)
print("Total arguments passed:", n)
# Arguments passed
print("\nName of Python script:", sys.argv[0])
 
print("\nArguments passed:", end = " ")
for i in range(1, n):
    print(sys.argv[i], end = " ")
print("\n======================================")

tts = gTTS(sys.argv[3], lang=sys.argv[1])

tts.save(sys.argv[2])

