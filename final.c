#include<stdio.h>
#include<stdlib.h>
struct nd
{
	int x;
	int y;
	int id;
	int flag;
	struct nd *left;
	struct nd *down;
	struct nd *right;
	struct nd *up;	
};
typedef struct nd node;
node **nodelist;
int *faultlist,*suspectedfaultlist;
int clockcycle=0;
void displayVisitedNodeBiochip(int m,int n)
{
	printf("\nBiochip info:\n");
	int i,j,k=0;
	for(i=0;i<m;i++)
	{
		printf("\n");
		for(j=0;j<n;j++,k++)
			printf("%5d",nodelist[k]->flag);
	}
}
void displayfaultylist(int *a,int n)
{
	int i;
	for( i=1;i<=n;i++)
		printf("\t%d ",a[i]);
}  
int left(int j)
{
	j=j-1;
	if(nodelist[j]->left)
		return nodelist[j]->left->id;
	else
		return 0;
}
int right(int j)
{
	j=j-1;
	if(nodelist[j]->right)
		return nodelist[j]->right->id;
	else
		return 0;
}
int up(int j)
{
	j=j-1;
	if(nodelist[j]->up)
		return nodelist[j]->up->id;
	else
		return 0;
}
int down(int j)
{
	j=j-1;
	if(nodelist[j]->down)
		return nodelist[j]->down->id;
	else
		return 0;
}
int set(int j)
{
	if(j<=0)
		return 0;
	j=j-1;
	if(nodelist[j]->flag==0 || nodelist[j]->flag==1)
	{
		nodelist[j]->flag=1;
		return j+1;	
	}
	else
		return 0;//faulty list
}
int reset(int* a, int k)
{
	int i;
	for(i=0;i<k;i++)
	{
		if(nodelist[a[i]-1]->flag!=2)
		nodelist[a[i]-1]->flag=0;
		a[i]=0;
	}
}
int nodeStatus(int j)
{
	if(j<=0)
		return 0;
	j=j-1;
	if(nodelist[j]->flag==2 || nodelist[j]->flag==1)
	{
		return 0;
	}
	else
		return 1;	
}
int* suspectednodelist(int m,int n)
{
	int k=0,count=1,i,j;
	int *a=(int*)malloc((m*n)*sizeof(int));
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++,k++)
		{
			if(nodelist[k]->flag==1)
				continue;
			else
				a[count++]=nodelist[k]->id;
		}
	}
	a[0]=count-1; //Count total number of expected faulty nodes
	return a;				
}
void peripheralTestRD(int m,int n,int j)
{
    int *a=(int*)malloc(m*n*sizeof(int));
	int k=0,flag=0,id=0;
	set(j);
    int i;
	for(i=j;i<n;i++)
	{
		id=right(i);
		if(id!=0)
		{
			if(nodelist[id-1]->flag==0)
			a[k++]=id;
	 		if(set(id)==0)
	 		{
	 			flag=1;
	 			goto label;
			}
				
		}
	}
	for(i=1;i<m;i++)
	{
		id=down(i*n);
		if(id!=0)
		{
			if(nodelist[id-1]->flag==0)
			a[k++]=id;
		//	printf("\nNode is:%d",id);
	 		if(set(id)==0)
	 		{
	 			flag=1;
	 			goto label;
			}
				
		}
	}
	label:;
	if(flag==1)
		reset(a,k);
	free(a);
}
void peripheralTestDR(int m,int n,int j)
{
    int *a=(int*)malloc(m*n*sizeof(int));
	int k=0,i,flag=0,id=0;
	set(j);
	for(i=j;i<m;i++)
	{
		id=down(n*i+1-n);
		if(id!=0)
		{
			if(nodelist[id-1]->flag==0)
			a[k++]=id;
	 		if(set(id)==0)
	 		{
	 			flag=1;
	 			goto label;
			}
				
		}
	}
	for(i=(m-1)*n+1;i<m*n;i++)
	{
		id=right(i);
		if(id!=0)
		{
			if(nodelist[id-1]->flag==0)
			a[k++]=id;
	 		if(set(id)==0)
	 		{
	 			flag=1;
	 			goto label;
			}
				
		}
	}
	label:;
	if(flag==1)
		reset(a,k);
	free(a);
}
void RDTest(int m,int n,int j)
{
	int i,k=0,flag=0,r=0;
	int *a=(int*)malloc(m*n*sizeof(int));
	
	for(i=j;i<=m-1;i=i+2)
	{
	//	printf("\n%d string:",i);
		k=1;
		for(j=1;j<=i;j++)
		{
			k=down(k);
		//	printf("\nNode:%d",k);
			if(nodeStatus(k))
				a[r++]=k;
			if(!set(k)) 
			{
				flag=1;
				goto label;
			}
		}
			
		for(j=1;j<=n-i-1;j++)
		{
			k=right(k);
			if(nodeStatus(k))
			a[r++]=k;
		//	printf("\nNode:%d",k);
			if(!set(k)) 
			{
				//a[r++]=k;
				flag=1;
				goto label;
			}
		}
		for(j=1;j<=m-i-1;j++)
		{
			k=down(k);
			if(nodeStatus(k))
			a[r++]=k;
			//printf("\nNode:%d",k);
			if(!set(k)) 
			{
				//a[r++]=k;
				flag=1;
			}
		}
		for(j=1;j<=i;j++)
		{
			k=right(k);
			if(nodeStatus(k))
			a[r++]=k;
			//printf("\nNode:%d",k);
			if(!set(k)) 
			{
				//a[r++]=k;
				flag=1;
				goto label;
			}
		}
		label:;
		if(flag==1)
			reset(a,r);
		flag=0;
		r=0;
	}
	free(a);
	
}
void DRTest(int m,int n,int j)
{
	int i,k=0,flag=0,r=0;
	int *a=(int*)malloc(m*n*sizeof(int));
	for(i=j;i<=n-2;i=i+2)
	{
	//	printf("\n%d string:",i);
		k=1;
		for(j=1;j<=n-i-1;j++)
		{
			k=right(k);
		//	printf("\nNode:%d",k);
			if(nodeStatus(k))
				a[r++]=k;
			if(!set(k)) 
			{
				//a[r++]=k;
				flag=1;
				goto label;
			}
		}
		for(j=1;j<=i;j++)
		{
			k=down(k);
		//	printf("\nNode:%d",k);
			if(nodeStatus(k))
				a[r++]=k;
			if(!set(k)) 
			{
				//a[r++]=k;
				flag=1;
				goto label;
			}	
		}
		for(j=1;j<=i;j++)
		{
			k=right(k);
		//	printf("\nNode:%d",k);
			if(nodeStatus(k))
				a[r++]=k;
			if(!set(k)) 
			{
				//a[r++]=k;
				flag=1;
				goto label;
			}
		}
		for(j=1;j<=m-i-1;j++)
		{
			k=down(k);
		//	printf("\nNode:%d",k);
			if(nodeStatus(k))
				a[r++]=k;
			if(!set(k)) 
			{
				//a[r++]=k;
				flag=1;
				goto label;
			}	
		}
		label:;
		if(flag==1)
			reset(a,r);
		flag=0;
		r=0;
	}
	free(a);
}
void add(int *s,int n)//add the number n at the last position of array s[]
{
	int c=s[0]+1;
	s[0]=c;
	s[c]=n;
}
int find(int *s,int n)//find n is present in the array s[] or not
{
	int c=s[0];
	int i;
	for(i=1;i<=c;i++)
	{
		if(s[i]==n)
			return 1;
	}
	return 0;
}
int tn,fg;
void down_move(int r)
{
	int k;
	for(k=1;k<r;k++)
	{
		tn=down(tn);
		clockcycle++;
	}

}
int right_move(int c)
{
	int jump=0,k;
	for(k=1;k<c;k++)
	{
		if(find(faultlist,right(tn))==1)//for faulty nodes by pass is done
		{
			int t1=up(tn);
			int t2=right(t1);
			int t3=right(t2);
			if((find(faultlist,t1)==1)||(find(faultlist,t2)==1)||(find(faultlist,t3)==1))
			{
				tn=down(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=right(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
					tn=right(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=up(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				k++;
				jump=0;
			}
			else
			{
				tn=up(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=right(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=right(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=down(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				k++;
				jump=1;
			}
		}
		else
		{
			tn=right(tn);
			if(nodelist[tn-1]->flag==2)
				fg=1;
			clockcycle++;
			jump=0;
		}
	}
	return jump;
}
void right_move2(int c)
{
	int k;
	for(k=1;k<c;k++)
	{
		tn=right(tn);
		clockcycle++;
	}
}
int down_move2(int r)
{
	int jump=0,k;
	for(k=1;k<r;k++)
	{
		if(find(faultlist,down(tn))==1)//bypass is done for faulty nodes
		{
			int t1=left(tn);
			int t2=down(t1);
			int t3=down(t2);
			if((find(faultlist,t1)==1)||(find(faultlist,t2)==1)||(find(faultlist,t3)==1))
			{
				tn=right(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=down(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=down(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
		            fg=1;
				tn=left(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				k++;
				jump=0;

			}
			else
			{
				tn=left(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=down(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=down(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				tn=right(tn);
				clockcycle++;
				if(nodelist[tn-1]->flag==2)
					fg=1;
				k++;
				jump=1;
			}
		}
		else
		{
			tn=down(tn);
			clockcycle++;
			if(nodelist[tn-1]->flag==2)
				fg=1;
			jump=0;
		}
	}
	return jump;

}
void square(int m,int n)
{
	int current,min; 
	if(m>n)
		min=n;
	else
		min=m;
	int i,j,k;
	int r,c;
	int bypass1;
	for(i=2;i<min;i++)
	{
		int *fr,*fc;
		fr=(int*)malloc(m*sizeof(int));//fr stores the id of the faulty nodes in the row
		fc=(int*)malloc(n*sizeof(int));//fc stores the id of the faulty nodes in the column
		fr[0]=0;
		fc[0]=0;
		current=(i-1)*n+i;
		for(j=i;j<n;j++)//finds the faulty nodes in that row
		{
			if(find(suspectedfaultlist,current)==1)
				add(fr,current);
			current=right(current);
		}
		current=i*n+i;
		for(j=i+1;j<m;j++)//finds the faulty nodes in that column
		{
			if(find(suspectedfaultlist,current)==1)
				add(fc,current);
			current=down(current);
		}
		if((fr[0]==0)&&(fc[0]==0))//if no unvisited electrodes are present in that particular row and column then we move on to the next row and column
			continue;
		for(j=1;j<=fr[0];j++)//each unvisited electrode in fr row is visited individually
		{
			current=fr[j];
			bypass1=0;
			fg=0;//
			r=nodelist[current-1]->x;
			c=nodelist[current-1]->y;
			tn=1;//present position of droplet on biochip
		
			down_move(r);
			int jump=0;	
			
			jump=right_move(c);
			
			int stepup=0;
			for(k=1;k<=(n-c);k++)
			{
				int t=right(tn);
				if((find(faultlist,t)==1)||(find(fr,t)==1))//for faulty and unvisited nodes bypass is done
				{
					
					if(find(faultlist,up(tn))==1)
					{
						tn=down(tn);
						clockcycle++;
						if(nodelist[tn-1]->flag==2)
							fg=1;
						tn=right(tn);
						clockcycle++;
						if(nodelist[tn-1]->flag==2)
							fg=1;
						tn=right(tn);
						clockcycle++;
						if(nodelist[tn-1]->flag==2)
							fg=1;
						tn=up(tn);
						clockcycle++;
						if(nodelist[tn-1]->flag==2)
							fg=1;
						k++;
						bypass1=1;
					}
					else
					{
							if(jump==0)
							{
								tn=up(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;
								stepup++;
								k--;
							}
							else
							{
								tn=down(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;
								tn=right(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;
								tn=right(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;
								tn=up(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;
								k++;
								bypass1=1;	
							}

					}
				}
				else
				{
					tn=right(tn);
					clockcycle++;
					if(nodelist[tn-1]->flag==2)
						fg=1;
				}
				jump=0;
			}

			down_move(m-r+stepup+1);

			if(fg==1 && bypass1==1)
			{
				tn=1;
				fg=0;
				down_move(r);
				jump=0;
				jump=right_move(c);
				stepup=0;
				for(k=1;k<=(n-c);k++)
				{
					 int t=right(tn);
					if((find(faultlist,t)==1)||(find(fr,t)==1))//for faulty and unvisited nodes bypass is done
					{
					
						if(find(faultlist,up(tn))==1)
						{
							tn=right(tn);
							clockcycle++;
							if(nodelist[tn-1]->flag==2)
								fg=1;
							
						}
						else
						{
							if(jump==0)
							{
								tn=up(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;
								stepup++;
								k--;
							}
							else
							{
								tn=right(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;
							}

						}
					}
					else
					{
						tn=right(tn);
						clockcycle++;
					}
					jump=0;
				}
				down_move(m-r+stepup+1);

			}
			if(fg==1)
				add(faultlist,current);
			else
				set(current);
		}
		for(j=1;j<=fc[0];j++)//each unvisited electrode in fr column is visited individually
		{
			current=fc[j];
			
			fg=0;//flag
			r=nodelist[current-1]->x;
			c=nodelist[current-1]->y;
			tn=1;//tn shows the id of electrode currently switched on
			bypass1=0;
			right_move2(c);
			int jump=0;
			jump=down_move2(r);
			
			int stepleft=0;
			for(k=1;k<=(m-r);k++)
			{
				int t=down(tn);
				if((find(faultlist,t)==1)||(find(fc,t)==1))//bypass is done for faulty and unvisited nodes
				{
					int t1=left(tn);
					if(find(faultlist,t1)==1)
					{
						tn=right(tn);
						clockcycle++;
						if(nodelist[tn-1]->flag==2)
							fg=1;
						tn=down(tn);
						clockcycle++;
						if(nodelist[tn-1]->flag==2)
							fg=1;
						tn=down(tn);
						clockcycle++;
						if(nodelist[tn-1]->flag==2)
							fg=1;
						tn=left(tn);
						clockcycle++;
						if(nodelist[tn-1]->flag==2)
							fg=1;
						k++;
						bypass1=1;
					}
					else
					{	
						if(jump==0)
						{
							tn=left(tn);
							clockcycle++;
							if(nodelist[tn-1]->flag==2)
								fg=1;
							stepleft++;
							k--;
						}
						else
						{
							tn=right(tn);
							clockcycle++;
							if(nodelist[tn-1]->flag==2)
								fg=1;
							tn=down(tn);
							clockcycle++;
							if(nodelist[tn-1]->flag==2)
								fg=1;
							tn=down(tn);
							clockcycle++;
							if(nodelist[tn-1]->flag==2)
								fg=1;
							tn=left(tn);
							clockcycle++;
							if(nodelist[tn-1]->flag==2)
								fg=1;
							k++;
							bypass1=1;
						}
					}
				
				}
				else
				{
					tn=down(tn);
					if(nodelist[tn-1]->flag==2)
						fg=1;
					clockcycle++;
				}
				jump=0;
			}
			
			right_move2(n-c+stepleft+1);

			if(fg==1 && bypass1==1)
			{
				tn=1,fg=0;
				right_move2(c);
				jump=0;
				jump=down_move2(r);
				stepleft=0;
				for(k=1;k<=(m-r);k++)
				{
					int t=down(tn);
					if((find(faultlist,t)==1)||(find(fc,t)==1))//bypass is done for faulty and unvisited nodes
					{
						int t1=left(tn);
						if(find(faultlist,t1)==1)
						{
							tn=down(tn);
							clockcycle++;
							if(nodelist[tn-1]->flag==2)
								fg=1;
					
						}
						else
						{	
							if(jump==0)
							{
								tn=left(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;
								stepleft++;
								k--;
							}
							else
							{
								tn=down(tn);
								clockcycle++;
								if(nodelist[tn-1]->flag==2)
									fg=1;		
							}
						}
					
					}
					else
					{
						tn=down(tn);
						if(nodelist[tn-1]->flag==2)
							fg=1;
						clockcycle++;
					}
					jump=0;
				}
				right_move2(n-c+stepleft+1);
			}
			if(fg==1)
				add(faultlist,current);
			else
				set(current);
		}
	free(fr);
	free(fc);
	}
}	

int main()
{
	FILE *fp;
	int n;
	char s[30];
	int row[8],m,i,j;
    printf("Enter your input file name with .ext:");
    scanf("%s",s);
	fp=fopen(s,"r");
	fscanf(fp,"%d ",&m);
	fscanf(fp,"%d ",&n);
	printf("Biochip size is %d*%d",m,n);
	printf("\nEnter total number of nodes:%d\n",m*n);
		nodelist=(node**)malloc(m*n*sizeof(node*));
	for(i=0;i<m*n;i++)
		nodelist[i]=(node*)malloc(sizeof(node));
	for(i=0;i<m*n;i++)
	{
		for(j=0;j<8;j++)    //Input file's each row contains 8 values  
		{
			fscanf(fp,"%d ",&row[j]);
		}
		nodelist[i]->x=row[0];
		nodelist[i]->y=row[1];
		nodelist[i]->id=row[2];
		nodelist[i]->flag=row[3];	
		nodelist[i]->left=(row[4]==0)?NULL:nodelist[row[4]-1];
		nodelist[i]->down=((row[5]==0)?NULL:nodelist[row[5]-1]);
		nodelist[i]->right=((row[6]==0)?NULL:nodelist[row[6]-1]);
		nodelist[i]->up=((row[7]==0)?NULL:nodelist[row[7]-1]);
	}
	faultlist=(int *)malloc((m*n)*sizeof(int));
	faultlist[0]=0;
	
	//Peripheral Test
	peripheralTestRD(m,n,1);
	peripheralTestDR(m,n,1);

	//RD Test
	RDTest(m,n,1);
	RDTest(m,n,2);
	printf("\nAfter RD test : \n");
	displayVisitedNodeBiochip(m,n);
	int ttime=4*m+n-5;
	suspectedfaultlist=suspectednodelist(m,n);
	if(suspectedfaultlist[0]==0)
	{
		printf("\nNo faults are detected :)\n");
		printf("Testing time is %d clockcycle\n",ttime );
	}
	else
	{
		//DR Test
		DRTest(m,n,1);
		DRTest(m,n,2);
		printf("\nAfter DR test : \n");
		displayVisitedNodeBiochip(m,n);
		suspectedfaultlist=suspectednodelist(m,n);
		clockcycle=ttime+m+4*n-11;
		//displayfaultylist(suspectedfaultlist,suspectedfaultlist[0]); //faultylist[0] denotes the number of expected faults in the biochip
		square(m,n);
		printf("\nAfter unit test with bypassing :\n");
		displayVisitedNodeBiochip(m,n);
		printf("\n the faulty nodes are :  ");
		for(int i=1;i<=faultlist[0];i++)
			printf("(%d,%d)\t",nodelist[faultlist[i]-1]->x,nodelist[faultlist[i]-1]->y);
		printf("\n");
		printf("Testing time is %d clockcycle\n",ttime );
		printf("The diagnosis time is %d clockcycle\n",clockcycle-ttime );
	}
	fclose(fp);

}