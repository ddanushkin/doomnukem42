from pydub import AudioSegment
import sys
import os

path_results = 'out/'
path_originals = 'in/'
filename_counter = 0
for filename in os.listdir(path_originals):
	print(filename)
	AudioSegment.from_file(path_originals + filename).export(path_results + str(filename_counter) + ".ogg", format="ogg", bitrate="64k")
	filename_counter += 1