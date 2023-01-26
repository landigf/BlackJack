#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
 * Programma completo per giocare a BlackJack, con registrazione e login
 * Si sceglie quanto depositare, e al login viene detto il saldo attuale
 * BlackJack completo, con l'asso, raddoppio, split
 * 
 *
 * 
 * PREMERE INVIO PER UTILIZZARE LA PUNTATA PRECEDENTE
*/

struct user_struct {
    char password[20];
    char conf_pass[20];
    char username[30];
    int bank;
} user;

void sing_up();
void login();
void game(int, int, int, int);
int random();
int draw(int [][52]);
void MischiaMazzo(int [][52]);
void cls();

int main() {
	FILE *fp;
    fp = fopen("storage.txt", "r");  // "r" indica che il file verrà aperto in modalità di lettura
    if (fp == NULL) {
        // Il file non esiste, quindi viene creato
        fp = fopen("storage.txt", "w");  // "w" indica che il file verrà aperto per la scrittura
    }
    fclose(fp);  // chiude il file
    int mazzo = 52;
    int deck[2][52];
    MischiaMazzo(deck);
    int scelta;
    int error = 0;
    int score = 0;
    int split = 2;
    int asso = 0;
    int carta = 0;
    char ch;
    int pippo;
    label:
    do
    {
    printf("\t\t\t\t\t\t- Benvenuto Utente -\n");
    sleep(1);
    printf("Seleziona l'operazione da effettuare:\n");
    printf("1.Sing Up\n2.Login\n3.Account demo\n\n");
    printf("Scelta: ");
    scanf("%d", &scelta);

    cls();

    switch(scelta)
    {
        case 1:
            printf("\t\t\t\t\t\t- Sing Up -\n\n");
            sing_up();
            cls();
            printf("\t\t\t\t\t- Please restart BlackJack and login -\n\n");
            sleep(1);
            return 0;
            
        case 2:
            printf("\t\t\t\t\t\t- Login -\n\n");
            login();
            cls();
            printf("\t\t\t\t\t\t- Welcome %s -\n\n", user.username);
            sleep(1);
            error = 0;
            break;
            
        case 3:
            strncat(user.username, "Guest", 5);
            printf("\t\t\t\t\t\t- Welcome %s -\n\n", user.username);
            user.bank = 1000;
            sleep(1);
            error = 0;
            break;
            
        default:
            if (++error > 1)
            {
                printf("\t\t\t\t\t\t- Try again -\n\n");
                return 2;
            }
            printf("\t\t\t- ERROR: Inserisci una scelta valida! (1 - 2 - 3) -\n\n");
            sleep(2);
            cls();
            break;
    }
    } while (error != 0);
    do
    {
    printf("\n\tCarte: %d\t\t\t\t\t\t\t\t\t- Bank: %d$ -\n\n\n\n", mazzo, user.bank);
    printf("Quanto vorresti puntare? ");
    int puntata = 0;
    do
    {
        if (puntata != 0)
        {
            printf("\n\t\t\t\t\t- Puntata non valida! -\n\n");
            printf("Quanto vorresti puntare? ");
        }
        scanf("%d", &puntata);
        while((ch = getchar()) != '\n' && ch != EOF);
    } while (puntata > user.bank && puntata != 0);
    puntata = abs(puntata);
    if (score == 0) printf("\n\n\t\t\t\t\t\t- Shuffling... -\n\n");
    if (mazzo < 18)
    {
        printf("\n\n\t\t\t\t\t\t- Shuffling... -\n\n");
        MischiaMazzo(deck);
        mazzo = 52;
    }
    sleep(1);
    srand(time(0));
    int temp;
    
    int card_1 = draw(deck);
    mazzo--;
    if (card_1 > 10) temp = 10;
    else temp = card_1;
    int somma_player = temp;
    
    int card_dealer = draw(deck);
    mazzo--;
    if (card_dealer > 10) card_dealer = 10;
    int somma_dealer = card_dealer;
    
    user.bank -= puntata;
    game(puntata, somma_player, somma_dealer, mazzo);
    sleep(1);
    int card_2 = draw(deck);
    mazzo--;
    if (card_2 > 10) temp = 10;
    else temp = card_2;
    somma_player += temp;
    if (card_1 == 1 || card_2 == 1) asso++;
    if (somma_player + 10 == 21 && asso > 0)
    {
        somma_player = 21;
        game(puntata, somma_player, somma_dealer, mazzo);
        goto vincita;
    }
    sleep(1);
    game(puntata, somma_player, somma_dealer, mazzo);
    int chose;
    mark:
    printf("Quale mossa vuoi compiere? (1 - 2 - 3");
    if (card_1 == card_2) printf(" - 4)\n");
    else printf(")\n");
    printf("1. Carta\n");
    printf("2. Sto\n");
    printf("3. Raddoppio\n");
    if (card_1 == card_2) printf("4. Split\n");
    printf("\n\t\t\t\t\tScelta: ");
    scanf("%d", &chose);
    switch(chose)
    {
        case 1:
            do
            {
                push:
                carta = draw(deck);
                mazzo--;
                if (carta > 10) temp = 10;
                else temp = carta;
                somma_player += temp;
                if (carta == 1) asso++;
                if (asso > 0 && somma_player + 10 == 21)
                {
                    asso = 0;
                    somma_player == 21;
                    goto cambio;
                }
                errore:
                game(puntata, somma_player, somma_dealer, mazzo);
                if (somma_player > 21) break;
                else if (somma_player == 21) goto cambio;
                printf("Quale mossa vuoi compiere? (1 - 2)\n");
                printf("1. Carta\n");
                printf("2. Sto\n");
                printf("\n\t\t\t\t\tScelta: ");
                scanf("%d", &chose);
                if (chose == 2) goto cambio;
                else if (chose != 1) goto errore;
            } while (somma_player < 21);
            break;
        case 2:
            cambio:
            if (asso > 0 && somma_player <= 11) somma_player += 10;
            asso = 0;
            do
            {
                carta = draw(deck);
                mazzo--;
                if (carta > 10) temp = 10;
                else temp = carta;
                somma_dealer += temp;
                if (carta == 1 && somma_dealer <= 11) somma_dealer += 10;
                game(puntata, somma_player, somma_dealer, mazzo);
            } while (somma_dealer < 17);
            break;
        case 3:
            if (user.bank < puntata)
            {
                printf("\n\n\t\t\t\t- ERROR: Saldo insufficiente! -\n\n");
                sleep(2);
                game(puntata, somma_player, somma_dealer, mazzo);
                goto mark;
                break;
            }
            user.bank -= puntata;
            puntata *= 2;
            carta = draw(deck);
            mazzo--;
            if (carta > 10) temp = 10;
            else temp = carta;
            somma_player += temp;
            if (carta == 1) asso++;
            game(puntata, somma_player, somma_dealer, mazzo);
            goto cambio;
            break;
        case 4:
            printf("\n%d\n", asso);
            if (card_1 != card_2) goto base;
            puntata /= 2;
            smezzo:
            somma_player = card_1; 
            if (somma_player > 10) somma_player = 10;
            somma_dealer = card_dealer;
            if (somma_dealer > 10) somma_dealer = 10;
            split--;
            game(puntata, somma_player, somma_dealer, mazzo);
            goto push;
            break;
        default:
            base:
            printf("\n\n\t\t\t\t- ERROR: Inserisci una scelta valida! -\n\n");
            sleep(2);
            game(puntata, somma_player, somma_dealer, mazzo);
            goto mark;
            break;
    }
    vincita:
    if (somma_player > 21)
    {
        printf("\n\n\t\t\t\t\t\t- Il banco vince -\n\n");
    }
    else if (somma_dealer > 21)
    {
        printf("\n\n\t\t\t\t\t\t- Hai vinto! -\n\n");
        user.bank += puntata*2;
    }
    else if (somma_player > somma_dealer)
    {
        printf("\n\n\t\t\t\t\t\t- Hai vinto! -\n\n");
        user.bank += puntata*2;
    }
    else if (somma_player < somma_dealer)
    {
        printf("\n\n\t\t\t\t\t\t- Il banco vince -\n\n");
    }
    else
    {
        printf("\n\n\t\t\t\t\t\t- Pareggio ! -\n\n");
        user.bank += puntata;
    }
    sleep(2);
    cls();
    carta = 0;
    asso = 0;
    if (split == 1) goto smezzo;
    printf("\tCarte: %d\t\t\t\t- %s -\n\n", mazzo, user.username);
    score++;
    printf("\tPartite consecutive: %d", score);
    printf("\t\t\t\t\t\t\t\tBank: %d$\n\n\n", user.bank);
    sleep(1);
    printf("\n\n\n\t\t\t\t\t- Ti va di giocare ancora ? -\n\n");
    printf("1. Si\n2. No\n");
    printf("\n\n\tScelta: ");
    scanf("%d", &pippo);
    cls();
    printf("\t\t\t\t\t\t- Welcome %s -\n\n", user.username);
    } while (pippo == 1);
    cls();
    printf("\n\n\n\n\t\t\t\t- %s grazie per aver giocato a BlackJack -\n\n", user.username);
    
    sleep(4);
    return 0;
}

void sing_up() {
    FILE *fptr;
    printf("Inserisci le credenziali richieste:\n");
    printf("\nUsername: \t");
    scanf("%s", user.username);
    char ch;
    int k = 0;
    bool trovato = true;
    fptr = fopen("storage.txt", "r");
    if (fptr == NULL)
    {
        printf("- ERROR opening / creating the file! -\n\n");
        sleep(1);
        exit(1);
    }
    char verifica[50] = "User_Pass:";
    strncat(verifica, user.username, 20);
    while (!feof(fptr))
    {
        ch = fgetc(fptr);
        if (ch == verifica[k++])
        {
           if (verifica[k] == '\0')
           {
               trovato = !trovato;
               ch = fgetc(fptr);
               ch = fgetc(fptr);
               k = 0;
               break;
           }
        }   
        else k = 0;
    }
    if (!trovato) 
    {
        printf("\t\t\t\tUsername utilizzato, usarne uno diverso\n\n");
        sleep(2);
        exit(1);
    }
    fclose(fptr);
    printf("\n");
    
    int flag = 0;
    int i;
    for (i = 0; i < 3; i++)
    {
        printf("Password: \t");
        scanf("%s", user.password);
        printf("Confirm Pass: \t");
        scanf("%s", user.conf_pass);
        printf("\n");
        
        if (!strcmp(user.password, user.conf_pass))
        {
            do
            {
                if (flag++) printf("\t\t\t\t\tDeposito minimo: 100$");
                printf("\nDepostito($): ");
                scanf("%d", &user.bank);
            } while (user.bank < 100);
            break;
        }
        else
        {
            printf("\t\t\tLe Password inserite non corrispondono!!!\n\n");
        }
        if (i == 2) exit(1);
    }
    fptr = fopen("storage.txt", "a");
    if (fptr == NULL)
    {
        printf("- ERROR opening / creating the file! -\n\n");
        sleep(1);
        exit(1);
    }
    fprintf(fptr, "[User_Pass:%s|%s]%d$\n", user.username, user.password, user.bank);
    fprintf(fptr, "Username:%s\n", user.username);
    fprintf(fptr, "Password:%s\n", user.password);
    fprintf(fptr, "\n");
    fclose(fptr);
    
    cls();
}

void login()
{
    FILE *fptr;
    char ch;
    int i = 0;
    fptr = fopen("storage.txt", "r");
    if (fptr == NULL)
    {
        printf("- ERROR opening file! -\n\n");
        sleep(1);
        exit(1);
    }
    printf("Inserisci le credenziali richieste:\n\n");
    printf("Username: \t");
    scanf("%s", user.username);
    printf("\n");
    bool trovato = true;
    char verifica[50] = "User_Pass:";
    strncat(verifica, user.username, 20);
    while (!feof(fptr))
    {
        ch = fgetc(fptr);
        if (ch == verifica[i++])
        {
           if (verifica[i] == '\0')
           {
               trovato = !trovato;
               ch = fgetc(fptr);
               ch = fgetc(fptr);
               i = 0;
               break;
           }
        }   
        else i = 0;
    }
    if (trovato)
    {
        printf("Nessun utente registrato con quell'username!\n\n");
        exit(1);
    }
    int flag = 3;
    do
    {
        printf("Password: \t");
        scanf("%s", user.password);
        while (ch != ']' && flag == 3)
        {
            strncat(user.conf_pass, &ch, 1);
            ch = fgetc(fptr);
        }
        if (!strcmp(user.password, user.conf_pass))
        {
            break;
        }
        flag--;
        switch(flag)
        {
            case 2:
                printf("\n\t\t\t- Wrong Password! Please try again. (%d attempts left)", flag);
                break;
                
            case 1:
                printf("\n\t\t\t- Wrong Password! Please try again. (%d attempts left)", flag);
                break;
                
            default:
                printf("\n\t\t\t- Try with another Username and Password!!!\n\n");
                exit(1);
        }
        printf("\n");
    } while (flag > 0);
    char bank[20];
    int k = 0;
    int val;
    while (ch != '$')
    {
        ch = fgetc(fptr);
        bank[k] = ch;
        k++;
    }
    val = atoi(bank);
    user.bank = val;
    fclose(fptr);
    cls();
}

void game(int puntata, int somma_player, int somma_dealer, int mazzo) {
    cls();
    printf("\tCarte: %d\t\t\t\t- Bank: %d$ -\n\n", mazzo, user.bank);
    printf("\tPuntata: %d$\n\n\n\n", puntata);
    printf("Player: %d", somma_player);
    printf("\t\t\t\t\t\t\t\t\t\tDealer: %d\n\n", somma_dealer);
    sleep(1); 
}

int random()
{
    return (rand() % 52);
}

int draw(int deck[][52])
{
    int pos;
    do
    {
        pos = random();
    } while (deck[1][pos]);
    deck[1][pos] = 1;
    
    int card = deck[0][pos];
    return  card;
}

void MischiaMazzo(int deck[][52])
{
    int count = 1;
    int d;
    for (d = 0; d < 52; d++)
    {
        deck[0][d] = count++;
        deck[1][d] = 0;
        if (count == 14) count = 1;
    }
}

void cls()
{
    system("cls||clear");
}
