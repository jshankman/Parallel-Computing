//
// Torbert, 2.11.2013
//
int main(void)
{
	char command[150];
	//
	FILE* fout;
	//
	// ...
	//
	for(frame=0;frame<200;frame++)
	{
		//
		//
		//
		fout=fopen("tmpimg.ppm","w");
		//
		fprintf(fout,"P3\n%d %d\n255\n",numcols,numrows);
		//
		for(y=0;y<numrows;y++)
			for(x=0;x<numcols;x++)
			{
				//
				// ...
				//
				fprintf(fout,"%d %d %d\n",red,green,blue);
			}
		//
		fclose(fout);
		//
		//
		//
		ez+=0.05;
		pz+=0.05;
		//
		//
		//
		sprintf(command,"convert tmpimg.ppm frame%03d.png",frame);
		system(command);
		//
	}
	//
	return 0;
}
//
// end of file
//
