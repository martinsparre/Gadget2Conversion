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

void ReadGadget2File(char *, struct Particle **, unsigned int *);


int main(int argc, char *argv[])
{

	if(argc != 3){
		printf("Exiting. Correct use is ./GadgetToAscii GadgetFilename AsciiFilename\n");
		fflush(stdout);
		exit(0);
	}
	struct Particle *P1 = NULL;

	unsigned int Ndm1;

	ReadGadget2File(argv[1], &P1, &Ndm1);

	printf("#DM particles in the structure: %d\n", Ndm1);
	
	FILE *Outfile = fopen(argv[2],"w+");
	

	unsigned int i;
	
	fprintf(Outfile,"%u\n",Ndm1);

	for(i=0;i<Ndm1; i++){
		fprintf(Outfile,"%.10G\t",P1[i].Pos[0]);
		fprintf(Outfile,"%.10G\t",P1[i].Pos[1]);
		fprintf(Outfile,"%.10G\t",P1[i].Pos[2]);
		fprintf(Outfile,"%.10G\t",P1[i].Vel[0]);
		fprintf(Outfile,"%.10G\t",P1[i].Vel[1]);
		fprintf(Outfile,"%.10G\t",P1[i].Vel[2]);
		fprintf(Outfile,"%.10G\t",P1[i].Mass);
		fprintf(Outfile,"%u\t",P1[i].ID);
		fprintf(Outfile,"%.10G\n",P1[i].Pot);
	}
	
	fclose(Outfile);


	return 0;
};

void ReadGadget2File(char *Filename, struct Particle **P, unsigned int *Ndm)
{
	int i;
	int debug = 0;
	
	unsigned int BlockSize, BlockSize0, NPart[6], NPartTot = 0;
	double MassArray[6];

	float *x, *y, *z, *vx, *vy, *vz, *Mass, *V;
	unsigned int *ID;

	FILE *GadgetFile = fopen(Filename,"r");
	
	if(!GadgetFile)
	{
		printf("GadgetFile doesn't exist\n");
		fflush(stdout);
		exit('0');
	}

	fread(&BlockSize0, sizeof(BlockSize0), 1, GadgetFile);
	fread(NPart, sizeof(*NPart), 6, GadgetFile);
	fread(MassArray, sizeof(*MassArray), 6, GadgetFile);
	fseek(GadgetFile, 260, 0); //to find the block ending.
	fread(&BlockSize, sizeof(BlockSize), 1, GadgetFile);

	if(debug){
	printf("aa %d\n",BlockSize);
	printf("bb %d\n",BlockSize0);
	}

	*Ndm = NPart[1];

	for(i=0;i<6;i++)
	{
		NPartTot += NPart[i];
	}

	if(debug) printf("Total number of Particles: %d\n",NPartTot);
	
	x = malloc(NPartTot*sizeof(float));//opretter aloat-array med NPartTot floats...
	y = malloc(NPartTot*sizeof(float));
	z = malloc(NPartTot*sizeof(float));
	vx = malloc(NPartTot*sizeof(float));
	vy = malloc(NPartTot*sizeof(float));
	vz = malloc(NPartTot*sizeof(float));
	V = malloc(NPartTot*sizeof(float));
	Mass = malloc(NPartTot*sizeof(float));
	ID = malloc(NPartTot*sizeof(unsigned int));	

	if(!x  || !y || !z || !vx || !vy || !vz || !Mass || !V || !ID)
	{
		printf("couldn't allocate memory");
		fflush(stdout);
		exit(0);
	}

	//skip to end of header (2*4bytes for block declarations and 256 for headersize)
	fseek(GadgetFile, 264, 0);

	int k;
	fread(&BlockSize0, sizeof(BlockSize0), 1, GadgetFile);
	for(k=0;k<NPartTot;k++){
		fread(&x[k], sizeof(*x), 1, GadgetFile);
		fread(&y[k], sizeof(*y), 1, GadgetFile);
		fread(&z[k], sizeof(*z), 1, GadgetFile);
	}
	fread(&BlockSize, sizeof(BlockSize), 1, GadgetFile);

	if(debug){
	printf("a %d\n",BlockSize);
	printf("b %d\n",BlockSize0);
	}

	fread(&BlockSize0, sizeof(BlockSize0), 1, GadgetFile);
	for(k=0;k<NPartTot;k++){
		fread(&vx[k], sizeof(*vx), 1, GadgetFile);
		fread(&vy[k], sizeof(*vy), 1, GadgetFile);
		fread(&vz[k], sizeof(*vz), 1, GadgetFile);
	}
	fread(&BlockSize, sizeof(BlockSize), 1, GadgetFile);

	if(debug){
	printf("a0 %d\n",BlockSize);
	printf("b0 %d\n",BlockSize0);	
	}

	fread(&BlockSize0, sizeof(BlockSize0), 1, GadgetFile);
	fread(ID, sizeof(*ID), NPartTot, GadgetFile);
	fread(&BlockSize, sizeof(BlockSize), 1, GadgetFile);

	if(debug){
	printf("a1 %d\n",BlockSize);
	printf("b1 %d\n",BlockSize0);
	}

	fread(&BlockSize0, sizeof(BlockSize0), 1, GadgetFile);
	fread(Mass, sizeof(*Mass), NPartTot, GadgetFile);
	fread(&BlockSize, sizeof(BlockSize), 1, GadgetFile);

	if(debug){
	printf("a2 %d\n",BlockSize);
	printf("b2 %d\n",BlockSize0);	
	}

	fread(&BlockSize0, sizeof(BlockSize0), 1, GadgetFile);
	fread(V, sizeof(*V), NPartTot, GadgetFile);
	fread(&BlockSize, sizeof(BlockSize), 1, GadgetFile);

	if(debug){
	printf("a3 %d\n",BlockSize);
	printf("b3 %d\n",BlockSize0);

	printf("x0 %f\n",x[0]);
	for(i=0;i<6;i++){
		printf("Np %d\t%d\n",i,NPart[i]);
		printf("M %d\t%.40lf\n",i,MassArray[i]);
	}
	}

	*P = malloc(NPartTot*sizeof(struct Particle));
	struct Particle *Ptemp = *P;

	for(i=0; i<NPartTot;i++){
		Ptemp[i].Pos[0] = x[i];	// x[i] same as *(x+i)
		Ptemp[i].Pos[1] = y[i];
		Ptemp[i].Pos[2] = z[i];
		Ptemp[i].Vel[0] = vx[i];	
		Ptemp[i].Vel[1] = vy[i];
		Ptemp[i].Vel[2] = vz[i];
		Ptemp[i].Mass = Mass[i];
		Ptemp[i].Pot = V[i];
		Ptemp[i].ID = ID[i];
	}
};


