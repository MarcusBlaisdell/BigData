BEGIN { FS = "\t" }

{
	mainVid = $1
	for (i = 10; i <= NF; ++i) {
		print mainVid "\t" $i
	}
}

