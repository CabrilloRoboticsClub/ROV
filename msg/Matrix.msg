uint32 columns 0	# Width of matrix. Required.
uint32 rows 0			# Height of matrix. Optional; reports error if data length != columns*rows
float32[] data		# If final row of data does not reach all the way to the final column, initialize with zeroes.
