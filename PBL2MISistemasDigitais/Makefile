# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -std=c99 -I./CoLenda -Wall -Wextra

# Diretório onde a biblioteca está localizada
LIBDIR = CoLenda

# Arquivos fonte
SRCS = main.c $(LIBDIR)/colenda.c

# Arquivos objeto
OBJS = main.o $(LIBDIR)/colenda.o

# Nome do executável
EXEC = main

# Regras para compilar o executável
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

# Regra para compilar main.c
main.o: main.c $(LIBDIR)/colenda.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# Regra para compilar colenda.c
$(LIBDIR)/colenda.o: $(LIBDIR)/colenda.c $(LIBDIR)/colenda.h
	$(CC) $(CFLAGS) -c $(LIBDIR)/colenda.c -o $(LIBDIR)/colenda.o

# Limpeza dos arquivos objeto e executável
clean:
	rm -f $(OBJS) $(EXEC)
