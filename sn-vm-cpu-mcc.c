//SN-VM-CPU-MCC v.1.0.0
//use to compile gcc sn-vm-cpu-mcc.c -O3 -o cpu
//-O3 flag significantly increases optimisation causing far quicker computation
//2025 Jaden F C Bartlett

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#include <omp.h>


#define MATRIX_SIZE(d1,d2,t) ((d1)*(d2)*(sizeof(t)))
#define VECTOR_SIZE(d1,t)    ((d1)*(sizeof(t)))

#define MOFF(i, j, d2) ((d2)*(i)+(j))
#define MELT(x, i, j, d2) (*((x)+MOFF(i,j,d2)))

#define zmax(x, y) (((x)>(y))?(x):(y))
#define zmin(x, y) (((x)<(y))?(x):(y))


double seconds(void){
	struct timeval tp;
    struct timezone tzp;
    int i = gettimeofday(&tp, &tzp);
    return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

void zero_matr(int *x, int m, int n){
	memset(x, 0, MATRIX_SIZE(m, n, int));
}

void zero_mu(int *x, int m){
	memset(x, 0, VECTOR_SIZE(m, int)); 
}

void read_matr(int *x, int m, int n){
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			scanf("%d",&MELT(x, i, j, n));
		}	
	}
}

void read_vect(int *x, int m){
	for(int i=0; i<m; i++){
		scanf("%d", x+i);
	}
}

void cpy_matr(int *x, int m, int n, int *x1){
	memcpy(x1,x,MATRIX_SIZE(m,n,int));
}

void print_matr(int *x,int m,int n){
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			printf("%10d ", MELT(x,i,j,n));
		}
		printf("\n");
	}
}

void print_vect(int *x, int m){
	for(int i=0; i<m; i++){
		printf("%d ", x[i]);
	}
	printf("\n");
}


int main(int argc, char * argv[]){
	int m, n, mm;
	int t, ps;
	int *bsp, *bsw, *dsp, *dsw, *mu, *y;
	int k=0, dbg=0, maxk=-1;
	int f[2];
  
	double t1, dt;
  
	if(argc>1){ 
		dbg=atoi(argv[1]);
	}
	if(argc>2){
		maxk=atoi(argv[2]);
	}
  
	// read sns
  
    scanf("%d %d %d\n", &m, &n, &mm);
	if(dbg>0){
		printf("%d %d %d\n", m, n, mm);
	}
	
	bsp=(int*)malloc(MATRIX_SIZE(mm, n, int));
	bsw=(int*)malloc(MATRIX_SIZE(mm, n, int));
	dsw=(int*)malloc(MATRIX_SIZE(mm, n, int));
	dsp=(int*)malloc(MATRIX_SIZE(mm, n, int));
	mu=(int*)malloc(VECTOR_SIZE(m, int));
	y=(int*)malloc(MATRIX_SIZE(mm, n, int));
	
	if(bsp==NULL || dsp==NULL || bsw==NULL || dsw==NULL || mu==NULL || y==NULL){
		printf("*** error: not enough memory\n");
		exit(3);
	}
  
	read_matr(bsp, mm, n);
	if(dbg>2){
		printf("bsp:\n");
		print_matr(bsp, mm, n);
	}

	read_matr(bsw, mm, n);
	if(dbg>2){
		printf("bsw:\n");
		print_matr(bsw, mm, n);
	}
	
	read_matr(dsp, mm, n);
	if(dbg>2){
		printf("dsp:\n");
		print_matr(dsp, mm, n);
	}
	
	read_matr(dsw, mm, n);
	if(dbg>2){
		printf("dsw:\n");
		print_matr(dsw, mm, n);
	}

	printf("initial mu:\n");
	read_vect(mu, m);
	print_vect(mu, m);
 
 
	t1=seconds();
  
    while(maxk==-1 || k<maxk){
		if(dbg>0){
			printf("sn-vm step k=%d\n", k); 
		}
	  
		//fire_arc
		#pragma omp parallel for private(t, ps)
		for(t = 0; t < n; t++){
			for(ps = 0; ps < mm; ps++){
				MELT(y, ps, t, n) = (MELT(bsw, ps, t, n)>0)? mu[MELT(bsp, ps, t, n)] / MELT(bsw, ps, t, n) : (MELT(bsw, ps, t, n)<0)? ((mu[MELT(bsp, ps, t, n)]>0)? 0: INT_MAX): INT_MAX;
			}
		}
	  
		if(dbg>1){
			printf("y#1:\n");                              
			print_matr(y, mm, n);
		} 
			
		//transition firing multiplicity
		//fire_trs
		#pragma omp parallel for private(t, ps)
		for(t = 0; t < n; t++){
			for(ps = 0; ps < mm; ps++){
				MELT(y, 0, t, n)=zmin(MELT(y, 0, t, n),MELT(y, ps, t, n));
			}
		}
	
		if(dbg>1){
			printf("y#2:\n");                              
			print_matr(y, mm, n);
		} 
		
		//choose firing transition (the first firable, since transitions are pre-sorted on priority)
		//y[0][0] = firing trs number; y[0][1] = firing trs multiplicity.
		//choose_f_trs
		
		int go=1;
		for(t = 0; t < n && go; t++){
			if(MELT(y, 0, t, n)>0){
				MELT(y, 0, 1, n)=MELT(y, 0, t, n); 
				MELT(y, 0, 0, n)=t; 
				go=0;
			}
		}
		
		if(go==1){
			MELT(y, 0, 0, n)=0; 
			MELT(y, 0, 1, n)=0;
		}
	
		memcpy(f, y, VECTOR_SIZE(2, int));
		
		if(dbg>1){
			printf("y#3:\n");                              
			print_matr(y, mm, n);
		} 
		
		if(f[1]){
			if(dbg>0){
				printf("fire %d in %d copies\n", f[0], f[1]);
			}
			//next marking
			//next_mu
			#pragma omp parallel for private(ps)
			for(ps = 0; ps < mm; ps++){
				if(MELT(bsw, ps, f[0], n)>0){
					mu[MELT(bsp, ps, f[0], n)]-=f[1]*MELT(bsw, ps, f[0], n);
				}
				if(MELT(dsw, ps, f[0], n)>0){
					mu[MELT(dsp, ps, f[0], n)]+=f[1]*MELT(dsw, ps, f[0], n);
				}
			}
			k++;
		}
		
		else{
			break;
		}
		
		if(dbg>0){
			printf("mu:\n");                              
			print_vect(mu, m);
		} 
	
		if(dbg>2){
			printf("bsp:\n");
			print_matr(bsp, mm, n);
			
			printf("bsw:\n");
			print_matr(bsw, mm, n);
			
			printf("dsp:\n");
			print_matr(dsp, mm, n);
		
			printf("dsw:\n");
			print_matr(dsw, mm, n);
		}
	}//end of while (firing transitions)
   
	dt=seconds()-t1;
	printf("it took %f s.\n", dt);
  
	//copy from device and print resulting marking
	printf("final mu:\n");  
	print_vect(mu, m);
  
	//free memory of device and host
	free(bsp);
	free(bsw);
	free(dsp); 
	free(dsw); 
	free(y);

}//end of main
