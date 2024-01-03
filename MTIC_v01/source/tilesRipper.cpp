#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]){
    int pos=0;
    unsigned char *buffer;
    size_t result;

    FILE *p = NULL;
    FILE *pout = NULL;
    if(argc == 3){
        p    = fopen(argv[1],"rb");
        pout    = fopen(argv[2],"rb");
    }

    if(p != NULL){
        // obtain file size:
        fseek (p , 0 , SEEK_END);
        long lSize = ftell (p);
        rewind (p);

        // allocate memory to contain the whole file:
        buffer = (unsigned char*) malloc (sizeof(char)*(lSize));
        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

        // copy the file into the buffer:
        result = fread (buffer,1,lSize,p);
        if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
        fclose(p);

        int pcounter = 1;

        pcounter = lSize / 64;
        char filename[100];
        for(int i=0; i<=pcounter; i++){
            sprintf(filename,"tiles/%s/tile%03d.bin",argv[2],i);
            pout = fopen(filename,"wb");
            if(pout == NULL){ free(buffer); return 0;}
            fwrite (buffer+i*64 , sizeof(char), 64, pout);
            fclose(pout);

        }

        free(buffer);
    }
    return 0;
}
