// Referenced Author: Kris Kaspersky
// This algorithm is awkward. This was done intentionally to allow significant scope for optimization

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#pragma pack(push,1)

// CONFIGURATION
// VARIABLE PARAMETER
#define ITER 10000                  // Maximum number of iterations

// CONSTANT PARAMETERS
#define MAX_CRYPT_LEN 200         // Maximum length of the ciphertext
#define KEY_SIZE 100000
#define PASS_X_LIM 1000
#define PASS_Y_LIM 100


struct key_attr {
  int  zi;
  long ki;
  char a[2];
  int  li;
};
struct key_attr key[KEY_SIZE];

char correct_plaintext[MAX_CRYPT_LEN]; // zeroed out plaintext to do a final check against;

int my_strlen(char *str)
{
    char *start = str;
    while(*(str++) != '\0');
    return str-start;
}

// Decrypting ciphertext using the found password
char* DeCrypt(char *ciphertext, char *pswd)
{
    int sub_func(int num)
    {
        switch (num)
        {
            case 0: return 1;
            case 1: return 3;
            case 2: return 2;
            case 3: return 4;
            case 4: return 7;
            case 5: return 6;
            case 6: return 10;
            case 7: return 5;
            case 8: return 9;
            case 9: return 16;
            case 10: return 15;
            case 11: return 14;
            case 12: return 11;
            case 13: return 17;
            case 14: return 12;
            case 31: return 102;
            default: return -1;
        }
    }

    char* buff = (char*)malloc(my_strlen(ciphertext));
    memcpy(buff, ciphertext, my_strlen(ciphertext));

    int px = 0, py = 0;

    do {
        int px = 0;
        do {
            int p = px * PASS_Y_LIM + py;
            struct key_attr key_i = key[p];
            
            int prf_int = (key_i.li ^ key_i.zi ^ key_i.ki) & (15);
            int mask = key_i.a[(prf_int+1) >> 4];
            
            if(prf_int == 15)
            {
                prf_int = 14;
            }
            
            int sub_result = sub_func(prf_int & mask) ^ pswd[p];

            buff[p % my_strlen(ciphertext)] ^= sub_result;
        } while ((++px < PASS_X_LIM));
    } while((++py < PASS_Y_LIM));
    
    return buff;
}

int my_memcmp(char *dst, char *src, unsigned count)
{
    while (count && (*(dst++) == *(src++))) count--;
    return count;
}

// Checking the password CRC
int CheckCRC(char *plaintext, int validCRC)
{
    // Calculating the password checksum
    int CalculateCRC(char *plaintext)
    {
        int a;
        int x = -1; // CRC calculation error

        for (a = 0; a < my_strlen(plaintext); a++)
            x += *(int *)(plaintext + a);
        
        free(plaintext);
        
        return x;
    }

    if ((CalculateCRC(plaintext) == validCRC) && (my_memcmp(plaintext, correct_plaintext, my_strlen(plaintext))))
        return 1;
    return 0;
}

// Processing the current password
void do_pswd(char *ciphertext, char *pswd, int validCRC, int progress)
{
  char *buff;

  // Checking the password CRC
  if (CheckCRC(DeCrypt(ciphertext, pswd), validCRC))
  { 
    // CRC match
    // Decrypting
    buff = DeCrypt(ciphertext, pswd);
    buff[10] = '\0'; // Restricting size of buffer printed

    // Displaying the decryption results
    printf("CRC %8X: try to decrypt: \"%s\"\n",
           CheckCRC(pswd, validCRC), buff);
    free(buff);
  }
}

// DO NOT MODIFY ANY CODE BELOW
int gen_pswd(char *ciphertext, char *pswd, int max_iter, int validCRC)
{
    long long int a;
    int p = 0;

    // Generating passwords
    for (a = 0; a < max_iter; a++)
    {

        // Processing the current password
        do_pswd(ciphertext, pswd, validCRC, (100 * a) / max_iter);

        // * Main loop of password generation *
        // Using the "counter" algorithm
        while ((++pswd[p]) > 'z')
        {
            pswd[p] = '!';
            p++;
            if (!pswd[p])
            {
                pswd[p] = ' ';
                pswd[p + 1] = 0;
            }
        } // end while (pswd)
        // Returning the pointer to initial position
        p = 0;
    } // end for(a)
    return 0;
}

// Displaying the number, using DOT as a delimiter
void print_dot(float per)
{

// * * * CONFIGURATION * * *
#define N 3        // Separating three positions
#define DOT_SIZE 1 // Size of the DOT delimiter
#define DOT ","    // Delimiter
  int a;
  char buff[666];
  sprintf(buff, "%0.0f", per);
  for (a = my_strlen(buff) - N; a > 0; a -= N)
  {

    memmove(buff + a + DOT_SIZE, buff + a, 66);
    if (buff[a] == ' ')
      break;
    else
      memcpy(buff + a, DOT, DOT_SIZE);
  }

  // Displaying on the screen
  printf("%s\n", buff);
}

long prf( long a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

void initialize_key()
{
    key[0].li = prf(0);
    key[0].zi = prf(1);
    key[0].ki = prf(2);

    for(int i=1; i<KEY_SIZE;i++)
    {
        key[i].li = prf(key[i-1].li);
        key[i].zi = prf(key[i-1].zi);
        key[i].ki = prf(key[i-1].ki);
        key[i].a[0] = 15;
        key[i].a[1] = 31;
    }

}

int main(int argc, char **argv)
{

  // Variables
  srand(0);
  initialize_key();

  char *buff;        // For reading data from the source file
  char *pswd;        // Currently tested password (needed by gen_pswd)
  int validCRC;      // For storing the original password CRC
  unsigned int t;    // For measuring the execution time
  int iter = ITER;   // Maximum number of passwords
  char *ciphertext;  // For storing the ciphertext

  // Title
  printf("= = = VTune profiling demo = = =\n"
         "=================================\n");

  // Memory allocation
  printf("memory malloc\t\t");
  buff = (char *)malloc(MAX_CRYPT_LEN+1);
  if (buff)
    printf("+OK\n");
  else
  {
    printf("-ERR\n");
    return -1;
  }

  // Getting the ciphertext for decryption
  printf("initializing from\t\tbuilt-in data\n");
  memset(buff,1,MAX_CRYPT_LEN);
  buff[MAX_CRYPT_LEN] = '\0';

  // Calculating CRC
  validCRC = 0;

  // Extracting the encrypted data
  ciphertext = buff;

  // Allocating memory for the password buffer
  printf("memory malloc\t\t");
  pswd = (char *)malloc(sizeof(char) * PASS_X_LIM * PASS_Y_LIM);

  memset(pswd, 0, sizeof(char) * PASS_X_LIM * PASS_Y_LIM); // Initialization

  if (pswd)
    printf("+OK\n");
  else
  {
    printf("-ERR\n");
    return -1;
  }
  // initial password and maximum number of iterations
  printf("max iter\t\t%d\n", iter);

  // Starting password enumeration
  printf("===\ntry search... wait!\n");
  t = clock();
  gen_pswd(ciphertext, pswd, iter, validCRC);
  t = clock() - t;

  // Output of the number of passwords per second
  printf("                                      \rPassword per sec:\t");
  print_dot(iter / (float)t * CLOCKS_PER_SEC);

//   free(ta);

  return 0;
}