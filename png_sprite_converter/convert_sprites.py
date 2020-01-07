from PIL import Image
import PIL
import sys
import os

path_results = 'out/'
path_originals = 'in/'
filename_counter = 0

for filename in os.listdir(path_originals):
	print(filename)
	original_image = Image.open(path_originals + filename).convert("RGBA")
	pixels = original_image.load()
	for y in range(original_image.size[1]):
		for x in range(original_image.size[0]):
			if pixels[x, y][3] < 255:
				pixels[x, y] = (255, 0, 255, 255)
	new_image = Image.new("RGBA", original_image.size, (255, 0, 255))
	new_image.paste(original_image, (0, 0), original_image)
	new_image.convert('RGBA').save(path_results + str(filename_counter) + ".bmp", "BMP")
	filename_counter += 1