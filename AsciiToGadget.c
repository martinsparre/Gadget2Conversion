#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//Authors: Morten Medici and Martin Sparre, 2012

struct Particle
{
	float Pos[3]; /* The position of the particle after the n'th timestep */
	float Vel[3];
	float Mass;
	float Pot;
	unsigned int ID;
}; 


void WriteGadget2File(char *, struct Particle **, unsigned int *);

int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("Exiting. Correct use is ./AsciiToGadget AsciiFilename GadgetFilename\n");
		fflush(stdout);
		exit(0);
	}	
	
	unsigned int Ndm1;


    FILE *Filename = fopen(argv[1],"r+");
    if(!Filename){
		printf("could not find ascii-file... exiting.\n");
		exit(0);
	}

	fscanf(Filename, "%u", &Ndm1);
	struct Particle *P1 = malloc(Ndm1*sizeof(struct Particle));

	
	unsigned int i;
    for (i=0;i<Ndm1;i++)
    {
			fscanf(Filename, "%f", &P1[i].Pos[0]);
			fscanf(Filename, "%f", &P1[i].Pos[1]);
			fscanf(Filename, "%f", &P1[i].Pos[2]);
			fscanf(Filename, "%f", &P1[i].Vel[0]);
			fscanf(Filename, "%f", &P1[i].Vel[1]);
			fscanf(Filename, "%f", &P1[i].Vel[2]);
			fscanf(Filename, "%f", &P1[i].Mass);
			fscanf(Filename, "%u", &P1[i].ID);			
			fscanf(Filename, "%f", &P1[i].Pot);
    }



	WriteGadget2File(argv[2], &P1, &Ndm1);


	return 0;
};


void WriteGadget2File(char *Filename, struct Particle **P, unsigned int *Ndm)
{	
	int i;
	struct Particle *Ptemp = *P;
	const int NumDM = *Ndm;

	FILE *OutputFile = fopen(Filename, "w");
	
	//Setting the header
	unsigned int header[64] = {0};
	
	header[0] = 0; //SPH
	header[1] = NumDM; //Halo
	header[2] = 0; //Disk
	header[3] = 0; //Bulge
	header[4] = 0; //Stars
	header[5] = 0; //Boundary
	for (i=0;i<6;i++) {
		header[6+i] = 0;
	}

	printf("size of header: %d\n",sizeof(header)); //checking length
	fflush(stdout);

	//Moving info from struct to arrays
	//define arrays:
	float *x = malloc(NumDM*sizeof(float));
	float *y = malloc(NumDM*sizeof(float));
	float *z = malloc(NumDM*sizeof(float));
	float *vx = malloc(NumDM*sizeof(float));
	float *vy = malloc(NumDM*sizeof(float));
	float *vz = malloc(NumDM*sizeof(float));
	unsigned int *ID = malloc(NumDM*sizeof(unsigned int));
	float *mass = malloc(NumDM*sizeof(float));
	float *Pot = malloc(NumDM*sizeof(float));
	
	//moving info:
	for (i=0; i<NumDM; i++){
		x[i] = Ptemp[i].Pos[0];
		y[i] = Ptemp[i].Pos[1];
		z[i] = Ptemp[i].Pos[2];
		vx[i] = Ptemp[i].Vel[0];
		vy[i] = Ptemp[i].Vel[1];
		vz[i] = Ptemp[i].Vel[2];
		ID[i] = Ptemp[i].ID;
		mass[i] = Ptemp[i].Mass;
		Pot[i] = Ptemp[i].Pot;
	}

	int blklen; // Block length
#define BLKLEN fwrite(&blklen, sizeof(blklen), 1, OutputFile);

	//Starts writting file
	blklen = sizeof(header);
	BLKLEN
	fwrite(&header, sizeof(header), 6, OutputFile);
	fseek(OutputFile, 260, 0); //to find the header ending.
	BLKLEN

	blklen = 3*sizeof(float)*NumDM;
	BLKLEN
	for(i=0;i<NumDM;i++){
		fwrite(&x[i], sizeof(float), 1, OutputFile);
		fwrite(&y[i], sizeof(float), 1, OutputFile);
		fwrite(&z[i], sizeof(float), 1, OutputFile);
	}
	BLKLEN

	blklen = 3*sizeof(float)*NumDM;
	BLKLEN
	for(i=0;i<NumDM;i++){
		fwrite(&vx[i], sizeof(float), 1, OutputFile);
		fwrite(&vy[i], sizeof(float), 1, OutputFile);
		fwrite(&vz[i], sizeof(float), 1, OutputFile);
	}
	BLKLEN

	blklen = sizeof(unsigned int)*NumDM;
	BLKLEN;
	for(i=0;i<NumDM;i++){
		fwrite(&ID[i], sizeof(unsigned int), 1, OutputFile);
	}
	BLKLEN;

	blklen = sizeof(float)*NumDM;
	BLKLEN;
	for(i=0;i<NumDM;i++){
		fwrite(&mass[i], sizeof(float), 1, OutputFile);
	}
	BLKLEN;
	BLKLEN;
	for(i=0;i<NumDM;i++){
		fwrite(&Pot[i], sizeof(float), 1, OutputFile);
	}
	BLKLEN;
	fclose(OutputFile); 
};
