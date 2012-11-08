/**
 * file		lib/string.c
 * autore	Filippo Bedendo Bordon
 * data		24-12-11
 * info		copia - incolla... (presto le guarderò)
**/

/*void strcpy(char *destinazione, const char *sorgente)
{
	do {
		*destinazione++ = *sorgente++;
	} while (*sorgente != 0);
}*/

// Restituisce la lunghezza di una stringa (-1)
unsigned int str_len(const char *str)
{
	register unsigned lung = 0;	
	// Finche non incontra il carattere di fine stringa aumenta il contatore
	while(*str++ != '\0') lung++;
	
	return lung;
}
		
		
unsigned int strcmp(const char *str1, const char *str2)
{
	int l_str1 = str_len(str1);
	int l_str2 = str_len(str2);
	
	// Se una ha più caratteri dell'altra esce subito
	if((l_str1 > l_str2) || (l_str1 < l_str2)) return 0;
	
	// Finche i caratteri sono uno a uno uguali continua
	while((*str1 == *str2) && (l_str1 >= 0))
	{
		// Incrementa i puntatori
		str1++;
		str2++;
		l_str1--;
	}
	// Se esce non avendo terminato il while
	// Allora non sono uguali e ritorna 0 se no 1
	if(l_str1 > 0) return 0;
	else return 1;
}

unsigned int strcpy(char *destinazione, const char *origine)
{
	unsigned int ctr = 0;
	while (origine[ctr])
	{
		destinazione[ctr] = origine[ctr++];
	}
	return ctr;
}

unsigned int str_replace(char *str, char *find, char *replace)
{
	unsigned int ctr, occorrenze;
	ctr = occorrenze = 0;
	while (str[ctr])
	{
		if (str[ctr] == *find )
		{
			ctr += strcpy(&str[ctr], replace);
			occorrenze++;
		}
		else
			ctr++;
	}
	return occorrenze;
}


		
		
unsigned int strcmpFILE(const char *str1, const char *str2)
{
	unsigned int str_len(const char *str)
	{
		register unsigned lung = 0;	
		// Finche non incontra il carattere di fine stringa aumenta il contatore
		while(*str != 0x20 && *str != 0x0) {*str++; lung++; }
		return lung;
	}
	int l_str1 = str_len(str1);
	int l_str2 = str_len(str2);
	
	// Se una ha più caratteri dell'altra esce subito
	if((l_str1 > l_str2) || (l_str1 < l_str2)) return 0;
	
	// Finche i caratteri sono uno a uno uguali continua
	while((*str1 == *str2) && (l_str1 >= 0))
	{
		// Incrementa i puntatori
		str1++;
		str2++;
		l_str1--;
	}
	// Se esce non avendo terminato il while
	// Allora non sono uguali e ritorna 0 se no 1
	if(l_str1 > 0) return 0;
	else return 1;
}
