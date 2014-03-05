/*
 *Guandi97
 *stdlib.h recreation
 */

#include <unistd.h>
#include <math.h>
#include <stdlib.h>
//debug
#include <stdio.h>

#define stdfun

typedef struct struct_file file;
int strsize(char*,char);						//returns incrementer;loops until delim is found
int ascint(char*);							//returns integer value of ascii string
char* intasc(int,int*);							//return length;convert int to ascii 
int ungReadi(int,char*,size_t);						//return # bytes read;read, and then clear buffer
int memcp(char*,char*,size_t);						//return # bytes copied;copy mem by byte
int readup(int,char*,char);						//return # bytes read;read up until delim
int buffwrite(char*,file*,size_t);					//return # bytes put in buffer;buffered output
int buffflush(file*);							//return # bytes flushed;flush file buffer


struct struct_file
{
	#define FBUFF 1024
	int fd;
	size_t index;
	char buff[FBUFF];
};

int strsize(char *str,char delim)
{
	int i=0;
	while(*(str+i)!=delim) i++;

	return i;
}
int ascint(char *str)
{
	int i=0,j=0;
	const int k=strsize(str,0x0)-1;
	int l=k;
	{
		if(*str=='-') i=1;
		for(i=i;i<k;i++)
		{
			j+=(pow(10,l)*(*(str+i)-0x30));
			l--;
		}
		if(*str=='-') j*=-1;
	}
	return j;
}
char* intasc(int source,int *b)
{
	int c=0,d,i=1,j,l;
	*b=-1;

	do
	{
		d=(int)(source/i);
		*b+=1;
		i*=10;
	}while(d!=0);

	char *s=malloc(*b);
	l=*b-1;

	for(i=pow(10,(double)l);i!=0;i/=10)
	{
		*(s+c++)=((int)(source/i)-(int)(source/(i*10))*10)+0x30;
	}
	*(s+c)=0x0;

	return s;
}
int ungReadi(int fd,char *buff,size_t size)
{
	#define UNGREADIBUFF 64
	char dump[UNGREADIBUFF];
	int i=read(fd,buff,size);
	if(i<size) return i;
	else 
	{
		if(buff[i-1]!=0xa) while(read(fd,dump,UNGREADIBUFF)>=UNGREADIBUFF);
	}

	return i;
}
int memcp(char *source,char *destin,size_t size)
{
	int i;
	for(i=0;i<(size);i++)
	{
		*(destin+i)=*(source+i);
	}
	return i;
}
int readup(int fd,char *destin,char delim)
{
	#define READUPBUFF 64
	int i,j,c=0;
	char buff[READUPBUFF];

	while(1)
	{
		i=read(fd,buff,READUPBUFF);
		if(i!=READUPBUFF) goto READLINEEND;

		i=0;
		while(buff[i]!=delim && i!=READUPBUFF) i++;
		if(i>=32)
		{
			memcp(buff,&(*(destin+c)),READUPBUFF);
			c+=READUPBUFF;
		}
	}

	READLINEEND:;
	j=0;
	while(buff[j]!=delim && j<i) 
	{
		j++;
	}
	memcp(buff,&(*(destin+c)),j);
	c+=j;

	return c;
}
int buffwrite(char *source,file *strmout,size_t size)
{
	int i;

	if((FBUFF-strmout->index)<size) 
	{
		buffflush(strmout);
	}

	i=memcp(source,&strmout->buff[strmout->index],size);
	size+=i;
	strmout->index=size;
	return i;
}
int buffflush(file *strmout)
{
	int i=write(strmout->fd,&strmout->buff,strmout->index);
	strmout->index=0;
	return i;
}

