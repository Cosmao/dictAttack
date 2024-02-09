## Att köra

För att hasha en fil förväntar sig CLI 2 argument, första är vilken fil den ska
läsa in för att börja hasha, andra är vilken typ av hash.

```
0 = MD5
1 = SHA256
2 = SHA512
```

Filen att hasha måste vara i formatet

```
lösenord,salt
lösenord,salt
lösenord,salt
```

clearTextSalt.txt i repo'n följer formatet.

Exempel för att köra hashern på clearTextSalt.txt med SHA512. Output blir
appendat med Hashed.txt

```
pwCrackerCLI clearTextSalt.txt 2
```

Att cracka lösenord kräver 4 argument, en fil med formatet över, en textfil med
vanliga lösenord (rockyou t.ex), hur många tråda och vilken typ av hash det är
(se ovan).

Exempel för att köra pwCrackerCLI på hashSaltMD5.txt med rockyou.txt som fil att
testa emot, 16 trådar och MD5.

```
pwCrackerCLI  hashSaltMD5.txt rockyou.txt 16 0
```

## Info

Gav upp lite på GUI och körde med CLI istället, finns flera executables att
bygga beroende på vad man vill ha. LoginMenu behöver inga argument, bara att
starta och köra. Finns ingen error hantering på argv så matar man in knas kommer
det crasha, om jag hade mer tid så hade jag fixat nått där.

Written by Emil Kool
