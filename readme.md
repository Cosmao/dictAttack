Gav upp lite på GUI och körde med CLI istället, finns flera executables att bygga

För att hasha en fil förväntar sig CLI 2 argument, första är vilken fil den ska läsa in för att börja hasha, andra är vilken typ av hash.
~~~
0 = MD5
1 = SHA256
2 = SHA512
~~~
filen att hasha måste vara i formatet 
~~~
{lösenord},{salt}\n
~~~

Att cracka lösenord kräver 4 argument, en fil med formatet över, en textfil med vanliga lösenord (rockyou t.ex), hur många tråda och vilken typ av hash det är (se ovan). 

Exempel
~~~
pwCrackerCLI  ../../../hashSaltMD5.txt ../../../rockyou.txt 16 0
~~~
För att börja knäcka hashedSaltMD5.txt som ligger 3 mappar upp med hjälp av rockyou, använder 16 trådar och MD5

Written by Emil Kool
