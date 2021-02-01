# Zaključani labirint

Nalazite se u labirintu, a kao što je slučaj i kod svakog drugog labirinta, cilj Vam je iz istog izaći.
Međutim, ovo nije običan labirint - u njemu se nalaze zaključana vrata i ključevi od kojih svaki otključava točno jedna vrata.

Zadatak je napisati funkciju  
  
- ``int solve(vector<vector<char>>& arr, int x, int y)``  
  
koja kao argumente prima referencu na polje ``arr`` koje predstavlja labirint, te x i y koordinate početne točke u labirintu. Funkcija vraća minimalan broj koraka potreban za izlaz iz labirinta.

Labirint je oblika:

<pre>
###########    
#a        #  
#########B#  
#         #  
#b        #  
#########A#
</pre>

gdje velika slova označavaju vrata, a odgovarajuća mala slova predstavljaju odgovarajući ključ koji ih otključava (dakle, ključ 'a' otključava vrata 'A').
*Hashtag* znakovi ('#') označavaju zidove, dok praznine (' ') označavaju prohodna polja.

## Primjer

Za labirint (točka predstavlja početnu točku samo zog jednostavnosti prikaza, u zadatku te točke neče biti u samom labirintu, već će biti zadana koordinatama ``x`` i ``y``):  

<pre>
###########    
#a        #  
#########B#  
#         #  
#b      . #  
#########A#
</pre>
rješavanje započinje odlaskom do ključa ``b`` (7 koraka),
  
<pre>
###########    
#a        #  
#########B#  
#         #  
#7654321. #  
#########A#
</pre>  
  
nastavlja se prolaskom do ključa ``a`` kroz vrata ``B`` koja sad možemo otključati (19 koraka),  
  
<pre>
###########    
#987654321#  
#########0#  
#        9#  
#.12345678#  
#########A#
</pre>

te se na koncu odlazi do izlaza (vrata ``A`` koja sad možemo otključati) (12 koraka).

<pre>
###########    
#.12345678#  
#########9#  
#        0#  
#        1#  
#########2#
</pre>  
  
Ukupan broj koraka je ``7 + 19 + 12 = 38``.

## Napomene
- Garantira se da će u svakom trenutku biti dostupan maksimalno jedan ključ koji još nije pokupljen te će svaki sljedeći ležati iza onih vrata koja trenutno dostupan ključ otključava.  
- Kad se ključ pokupi, on se smatra trajno skupljenim.
