BEGIN { FS = "\t" }

{
	if (NF >= 9) //Discludes all entries without the first 9 columns
	{
		for (i = 1; i < 9; ++i) {
			printf $i "\t"
		}
		print $9 
	}
}
