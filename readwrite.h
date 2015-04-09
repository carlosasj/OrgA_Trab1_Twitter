#ifndef READ_WRITE_H
#define READ_WRITE_H

// Lê um Tweet com base no RRN
void readRRN (TWEET *, FILE *, int);

// Lê o usuário armazenado no Tweet daquele RRN
void userRRN (char *, FILE *, int); // A Confirmar se a assinatura será assim

// Escreve o Tweet com base em um RRN *DANGER**BE CAREFUL*
void writeRRN (TWEET *, FILE *, int);

// Sobreposição do primeiro registro removido logicamente:
void writeTT (TWEET *, FILE *);

#endif