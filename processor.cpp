#include <bits/stdc++.h>
#define ll long long int
#define fo(i, n) for (ll i = 0; i < n; i++)

using namespace std;
char PC[3] = "00", A[4] = "000", B[4] = "001", ALU = '0', IR[3] = "00", MAR[3] = "00", MDR[4] = "000", SP[3] = "FF";
int carry = 0, zero = 0;
int i = 0;
char RAM[256][4];
int hextodec(char ch)
{
    int a = ch - '0';
    int b = ch - '7';
    if (ch < 'A')
        return a;
    else
        return b;
}
int hex2dec(string s)
{
    int sum = 0;
    int m = 1;
    for (int i = s.size()-1; i >= 0; i--)
    {
        sum += hextodec(s[i]) * m;
        m = m * 16;
    }
    return sum;
}

// C++ program to convert a decimal
// number to hexadecimal

// function to convert decimal to hexadecimal
string dec2hex(int n)
{
    // ans string to store hexadecimal number
    string ans = "";

    while (n != 0)
    {
        // remainder variable to store remainder
        int rem = 0;

        // ch variable to store each character
        char ch;
        // storing remainder in rem variable.
        rem = n % 16;

        // check if temp < 10
        if (rem < 10)
        {
            ch = rem + 48;
        }
        else
        {
            ch = rem + 55;
        }

        // updating the ans string with the character variable
        ans += ch;
        n = n / 16;
    }

    // reversing the ans string to get the final result
    int i = 0, j = ans.size() - 1;
    while (i <= j)
    {
        swap(ans[i], ans[j]);
        i++;
        j--;
    }
    return ans;
}

void showContent()
{
    cout << endl;
    cout << "PC=" << PC[0] << PC[1] << " "
         << "IR=" << IR[0] << IR[1] << " "
         << "MDR=" << MDR[0] << MDR[1] << MDR[2] << " "
         << "MAR=" << MAR[0] << MAR[1] << " "
         << "A=" << A[0] << A[1] << A[2] << " "
         << "SP=" << SP[0] << SP[1] << " "
         << "B=" << B[0] << B[1] << B[2] << endl;
    cout << endl;
}
void fetch()
{
    //     T0 : PC(E) , MAR(E,L)
    // T1 : PC(I) , RAM(E) , MDR(L)

    cout << "Fetch:" << endl;
    cout << "Enable Program Counter to address bus" << endl;
    cout << "Load Memory Address Register from address bus" << endl;
    for (int j = 0; j < 2; j++)
    {
        MAR[j] = PC[j];
    }

    cout << "Increment Program Counter" << endl;

    if (PC[1] != 'F')
        PC[1] = PC[1] + 1;
    else if (PC[0] == 'F' && PC[1] == 'F')
    {
        PC[0] = 0;
        PC[1] = 0;
    }
    else
    {
        PC[0] = PC[0] + 1;
        PC[1] = '0';
    }

    for (int j = 0; j < 3; j++)
    {
        MDR[j] = RAM[i][j];
    }
    cout << endl;
}

void decode()
{
    cout << "Decode:" << endl;

    //  MDR(E) ,IR(L) , MAR(L,E)      	// MAR <- MDR(7-0), IR <-MDR(8-11)

    // For MRI instruction
    if (MDR[0] != '7' && MDR[0] != 'F')
    {
        IR[0] = '0';
        IR[1] = MDR[0];
        MAR[0] = MDR[1];
        MAR[1] = MDR[2];
    }

    // For Non MRI instruction
    else
    {
        if (MDR[0] == 'F')
            IR[0] = '3';
        else
            IR[0] = '2';
        IR[1] = MDR[1];
    }
    cout << endl;
    showContent();
}

void solve(int n)
{

    i = 0;

    // Taking input in RAM

    for (int j = 0; j < n; j++)
    {
        for (int k = 0; k < 3; k++)
        {
            cin >> RAM[j][k];
        }
    }

    // is while loop me interrupt ki condition bhi add krni padegi sayad
    while (!(RAM[i][1] == 'F' && RAM[i][0]=='7')) // Means HLT instruction jab tak nhi aa jata
    {

        fetch();
        decode();

        // For MRI instruction

        if (!(RAM[i][0] == '7' || RAM[i][0] == 'F'))
        {
            switch (RAM[i][0])
            {

                // Direct LDA XX

            case '0':
            {
                int p = hex2dec(MAR);
                //  AC(L) , RAM(E), Reset
                for (int j = 0; j < 3; j++)
                {
                    A[j] = RAM[p][j];
                }
                showContent();
                break;
            }

                // Indirect LDA XX

            case '8':
            {
                int p = hex2dec(MAR);

                // MDR(L) , RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[p][j];
                }
                // MAR(L,E), MDR(E)
                for (int j = 0; j < 2; j++)
                {
                    MAR[j] = MDR[j + 1];
                }

                //  AC(L) , RAM(E), Reset
                int q = hex2dec(MAR);
                for (int j = 0; j < 3; j++)
                {
                    A[j] = RAM[q][j];
                }
                showContent();
                break;
            }

            // Direct STA XX
            case '1':
            {
                int p = hex2dec(MAR);
                // RAM(L) , AC(E) , Reset
                for (int j = 0; j < 3; j++)
                {
                    RAM[p][j] = A[j];
                }
                showContent();
                break;
            }

            // Indirect STA XX
            case '9':
            {
                int p = hex2dec(MAR);

                // MDR(L) , RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[p][j];
                }
                // MAR(L,E), MDR(E)
                for (int j = 0; j < 2; j++)
                {
                    MAR[j] = MDR[j + 1];
                }

                // RAM(L) , AC(E) , Reset
                int q = hex2dec(MAR);
                for (int j = 0; j < 3; j++)
                {
                    RAM[q][j] = A[j];
                }
                showContent();
                break;
            }

                // Direct ADM XX
            case '2':
            {
                int p = hex2dec(MAR);

                // MDR(L) , RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[p][j];
                }
                // AC <-AC + MDR
                for (int j = 0; j < 3; j++)
                {
                    A[j] = A[j] + MDR[j];
                }
                showContent();
                break;
            }

            // Indirect ADM XX
            case 'A':
            {
                int p = hex2dec(MAR);

                // MDR(L) , RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[p][j];
                }
                // MAR(L,E),MDR(E)
                for (int j = 0; j < 2; j++)
                {
                    MAR[j] = MDR[j + 1];
                }
                int q = hex2dec(MAR);

                // MDR(L) , RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[q][j];
                }
                // AC <-AC + MDR
                for (int j = 0; j < 3; j++)
                {
                    A[j] = A[j] + MDR[j];
                }
                showContent();
                break;
            }

                // Direct CALL XX
            case '3':
            {
                //  SP(D)

                if (SP[1] != 0)
                {
                    SP[1]--;
                }
                else
                {
                    SP[0]--;
                }

                // SP(E), MAR(E,L)
                for (int j = 0; j < 2; j++)
                {
                    MAR[j] = SP[j];
                }
                // RAM(L), PC(E)
                int p = hex2dec(MAR);
                for (int j = 0; j < 2; j++)
                {
                    RAM[p][j + 1] = PC[j];
                }
                // PC(L), MDR(E), Reset
                for (int j = 0; j < 2; j++)
                {
                    PC[j] = MDR[j + 1];
                }
                showContent();
                break;
            }

            // Indirect CALL XX
            case 'B':
            {
                int p = hex2dec(MAR);

                // MDR(L) , RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[p][j];
                }

                //  SP(D)

                if (SP[1] != 0)
                {
                    SP[1]--;
                }
                else
                {
                    SP[0]--;
                }

                // SP(E), MAR(E,L)
                for (int j = 0; j < 2; j++)
                {
                    MAR[j] = SP[j];
                }
                // RAM(L), PC(E)
                int q = hex2dec(MAR);
                for (int j = 0; j < 2; j++)
                {
                    RAM[q][j + 1] = PC[j];
                }
                // PC(L), MDR(E), Reset
                for (int j = 0; j < 2; j++)
                {
                    PC[j] = MDR[j + 1];
                }
                showContent();
                break;
            }

                // Direct JMP XX
            case '4':
            {
                // PC(L), MDR(E), Reset
                for (int j = 0; j < 2; j++)
                {
                    PC[j] = MDR[j + 1];
                }
                showContent();
                break;
            }

            // Indirect JMP XX
            case 'C':
            {
                int p = hex2dec(MAR);

                // MDR(L), RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[p][j];
                }
                // PC(L), MDR(E), Reset
                for (int j = 0; j < 2; j++)
                {
                    PC[j] = MDR[j + 1];
                }
                showContent();
                break;
            }

            // Direct JC XX
            case '5':
            {
                if (carry == 1)
                {
                    // PC(L), MDR(E), Reset
                    for (int j = 0; j < 2; j++)
                    {
                        PC[j] = MDR[j + 1];
                    }
                }
                showContent();
                break;
            }

            // Indirect JC XX
            case 'D':
            {
                int p = hex2dec(MAR);

                // MDR(L), RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[p][j];
                }
                if (carry == 1)
                {
                    // PC(L), MDR(E), Reset
                    for (int j = 0; j < 2; j++)
                    {
                        PC[j] = MDR[j + 1];
                    }
                }
                showContent();
                break;
            }
            // Direct JZ XX
            case '6':
            {
                if (zero == 1)
                {
                    // PC(L), MDR(E), Reset
                    for (int j = 0; j < 2; j++)
                    {
                        PC[j] = MDR[j + 1];
                    }
                }
                showContent();
                break;
            }

            // Indirect JZ XX
            case 'E':
            {
                int p = hex2dec(MAR);

                // MDR(L), RAM(E)
                for (int j = 0; j < 3; j++)
                {
                    MDR[j] = RAM[p][j];
                }
                if (zero == 1)
                {
                    // PC(L), MDR(E), Reset
                    for (int j = 0; j < 2; j++)
                    {
                        PC[j] = MDR[j + 1];
                    }
                }
                showContent();
                break;
            }
            default:
            cout<<"please enter valid Instruction"<<endl;
            break;
            }
            
        }

        // for non MRI instruction
        else if (RAM[i][0] == '7')//register based instruction
        {

            switch (RAM[i][1])
            {

            case '0':
        {
                int a = hex2dec(A);
                int b = hex2dec(B);
                a = a + b;
                string temp = dec2hex(a);
                string str;
                if(temp.size()<3)
                {
                    int n=3-temp.size();
                
                    for(int i=0;i<n;i++){
                        str[i]='0';
                    }
                    for(int i=n;i<3;i++){
                        str[i]=temp[i-n];
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    A[i] = str[i];
                }
                ALU = '0';
                showContent();
                break;
        }

            default:
            cout<<"enter a valid instruction";

                break;
            }

        }
         else if (RAM[i][0] == 'F') //I/O based instruction
        {

            switch (RAM[i][1])
            {

            }
        }

    i++;
    }
    }

int main()
{
    //    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

    // #ifndef ONLINE_JUDGE
    //    freopen("input.txt", "r", stdin);
    //    freopen("output.txt", "w", stdout);
    // #endif

    int t = 1;
    // cin >> t;
    // while (t--)
    // {
    //     solve();
    // }
    int n;
    cout << "no of instructions:";
    cin >> n;

    solve(n);

    return 0;
}
