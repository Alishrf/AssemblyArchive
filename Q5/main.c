#include <stdio.h>

void read_array(float a[],int n);
void print_array(float a[],int n);
/* Assume that the read_array and print_array
   functions have already been implemented. */

void correlate(float input[], int input_length,float filter[],
              int filter_length,float output[]);


int main(){

    int input_length, filter_length;
    scanf("%d%d",&input_length,&filter_length);

    float input[input_length],filter[filter_length];
    read_array(input,input_length);
    read_array(filter,filter_length);
    printf("input:\n");
    print_array(input,input_length);
    printf("\nfilter:\n");
    print_array(filter,filter_length);

    float output[input_length];
    correlate(input,input_length,filter,filter_length,output);
    printf("output:\n");
    print_array(output,input_length);
    
    return 0;
}



void read_array(float a[],int n){

    for(int i = 0 ; i < n ; i++ ){
        scanf("%f", &a[i]);
    }

}
void print_array(float a[],int n){

    for (int i = 0 ; i < n ; i++ ){
        printf("%f ",a[i]);
    }
}

/*void correlate(float input[], int input_length,float filter[], int filter_length,float output[]){

    for( int i = 0; i < input_length; i++){ //for every element in input
        
        float res = 0;
        
        for(int j = 0; j < filter_length; j++){ //for every element in filter

            int real = j - filter_length/2;

            if( i + real < 0 || i + real >= input_length){
                res += 0; //nothing
            }else{
                res += input[i+real]*filter[j];
            }
        }

        output[i] = res;
        
    }

}*/
