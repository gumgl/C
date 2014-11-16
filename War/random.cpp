#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

void main ()
{
   int secret, guess;

   //initialize random seed 
   srand ( (unsigned int)time(NULL) );
  
   //generate secret number 
   secret = rand() % 10 + 1;
  
   do
   {
      cout << "Guess the number (1 to 10)";
      cin >> guess;
  
      if ( secret < guess ) 
      {
          cout<<"The secret number is lower"<<endl;
      }
      else if ( secret > guess )
      { 
         cout<<"The secret number is higher"<<endl;
      } 
   } while ( secret!=guess );

   cout<<"You got it!"<<endl;
}
