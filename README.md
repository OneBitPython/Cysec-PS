# Cysec-PS
Cysec group DC project


Team name : <insert-smth-here>

need :
1) Multi-account support
2) accounts must be isolated ( tf isnt this obvious?)
3) any change of crit info must require PIN ( assume 8 digit pins? we can use unsigned int right?) -> point : we can use unsigned long long int for 8 digit pins , but then we must be sure to not cause any int overflow else cooked
4) Ability to change PIN
5) ----


Finally :
User Interaction Options:
1) Ability to change PIN
2) Create a New account ( im assuming)
3) Query to get Balance
4) Withdrawals ( with PIN )
5) Loans ( with PIN ) <- must come from admin account
6) Create a FD
7) Break FD

Admin: ("ADMIN"):
	CAN DO ANYTHING 🙁

An Account must Hold:
1)PAN,Credit Card , CVV



Eventually - we need to give : Source code in C , Binary 


Some stuff i wanna do:
Make it obscure but making the source code obscure is going to be diff so might skip this
Include redundant variables?
Use Cipher Stacking on the encrypted stuff ??




# To do List
1. Deposit money - 3 times you can try the PIN, and then log out of session
2. 

# Successful progress
1. Make new account function is inputting username and pin properly
2. Deposit money function is checking for user properly, and we're able to deposit money
3. Created a good function for clearing input stream to prevent buffer overflows