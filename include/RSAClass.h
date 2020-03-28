#include "bigint.h"

class RSAClass{
public:
    bigint p,q,n,e,d,phi;
    // p and q are prime numbers
    // n = p*q
    // phi = Totient(n)
    // e -> public Key
    // d -> private Key

    bigint generateRandomBigint(int length_of_number, bool end_with_one=true){
        string str_num = "1";
        //cout<<((rand()%10)+1)<<endl;
        for(int i=1; i<length_of_number; i++){
            str_num += to_string((rand()%10));
        }
        if(end_with_one){
            str_num[str_num.length()-1] = '1';
        }
        //cout<<str_num<<"\n";
        bigint num(str_num);
        return num;
    }

    bool miillerTest(bigint d, bigint n)
    {
        // Pick a random number in [2..n-2]
        // Corner cases make sure that n > 4

        bigint a = generateRandomBigint((rand()%n.size()));

        // Compute a^d % n
        bigint x = power(a, d, n);

        if (x == 1  || x == n-1)
            return true;

        // Keep squaring x while one of the following doesn't
        // happen
        // (i)   d does not reach n-1
        // (ii)  (x^2) % n is not 1
        // (iii) (x^2) % n is not n-1
        while (d != n-1)
        {
            x = (x * x) % n;
            d *= 2;

            if (x == 1)      return false;
            if (x == n-1)    return true;
        }

        // Return composite
        return false;
    }

    // It returns false if n is composite and returns true if n
    // is probably prime.  k is an input parameter that determines
    // accuracy level. Higher value of k indicates more accuracy.
    bool isPrime(bigint n, int k)
    {
        // Corner cases
        if (n <= 1 || n == 4)  return false;
        if (n <= 3) return true;

        // Find r such that n = 2^d * r + 1 for some r >= 1
        bigint d = n - 1;
        while (d % 2 == 0)
            d /= 2;

        // Iterate given nber of 'k' times
        for (int i = 0; i < k; i++)
            if (!miillerTest(d, n))
                return false;

        return true;
    }

    bigint power(bigint a, bigint b, bigint m){
        bigint ans = 1;
        while (b > 0)
        {
            if (b % 2 == 1)
                ans = (ans*a)%m;

            b /= 2;
            a = (a*a)%m;
        }
        return ans;
    }

    //generateRandomPrime using rabin-miller
    bigint generateRandomPrime(int length_of_prime){

        cout<<"Generating random prime number.."<<flush;
        for(int i=0;;i++){
            bigint num = generateRandomBigint(length_of_prime);
            if(isPrime(num, 100)){
                cout<<" Prime generated.\n"<<flush;
                return num;
            }
            cout<<"."<<flush;
        }
    }

    //Extended Euclidien Method
    tuple<bigint, bigint, bigint> extended_gcd(bigint a, bigint b)
    {
    	if (a == 0)
    		return make_tuple(b, 0, 1);

    	bigint gcd, x, y;

    	// unpack tuple  returned by function into variables
    	tie(gcd, x, y) = extended_gcd(b % a, a);

    	return make_tuple(gcd, (y - (b/a) * x), x);
    }

    bigint inv(bigint a, bigint m){
        bigint x;
        bigint y;
        tuple<bigint, bigint, bigint> t = extended_gcd(a, m);
        bigint g = get<0>(t);
        x = get<1>(t);
        y = get<2>(t);
        cout<<"g: "<<g<<endl;
        if (g != 1){
            return 0;
        }
        else
        {
            // m is added to handle negative x
            bigint res = (x%m + m) % m;
            return res;
        }
    }

    RSAClass(int key_len){
        p = generateRandomPrime(key_len);
        q = generateRandomPrime(key_len+1);
        n = (p*q);
        phi = ((p-1)*(q-1));
        e = calE();
        d = inv(e, phi);
    }

    bool prime(bigint n){
        for(bigint i = 2; i*i<=n; i=i+1){
            if(n%i==0) return false;
        }
        return true;
    }

    bigint calE(){
        if(phi>65537 && phi%65537!=0){
            return 65537;
        }
        for(bigint i=1000; i<=phi; i=i+1){
            if(prime(i) && phi%i!=0){
                return i;
            }
        }
    }

    bigint getN(){
        return n;
    }

    string decrypt(string crypt){
        bigint intcrypy;
        intcrypy.read(crypt);
        string message = power(intcrypy, d, n).to_string();
        string decrepted_message = "";
        if(message.length()%3==2) message = "0"+message;
        for(int i=0;i<(message.length()/3);i++){
            string curr = string(1,message[3*i])+string(1,message[3*i+1])+string(1,message[3*i+2]);
            //cout<<curr<<endl;
            int num = stoi(curr);
            decrepted_message += string(1, (char)num);
        }
        //std::cout<<decrepted_message<<std::endl;
        return decrepted_message;
    }
};
