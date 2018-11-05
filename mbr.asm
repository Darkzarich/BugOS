start:

	call loadfromHDD
	call loadimagefromHDD

	mov ah, 0
	mov al, 0x13
	int 0x10
	
	call pushpro
ret

pushpro:

	mov edi, dword[lfb_addr]
	mov esi, dword[lfb_pic]

	jmp 0x100:0
ret

loadimagefromHDD:
	mov ch, 0
	mov cl, 8; 3584
	mov dh, 0
	mov dl, 0x80; êîä ôëîïè äèñêîâîäà
	mov bx, 0x8000
	mov dword[lfb_pic], 0x8000
	mov ah, 2; ÷èòàòü
	mov al, 128; предел при 0x8000 равен 128 секторов
	int 0x13
ret

loadfromHDD:
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, 0x80; êîä ôëîïè äèñêîâîäà
	mov bx, 0x1000
	mov ah, 2; ÷èòàòü
	mov al, 8
	int 0x13
ret

lfb_addr dd 0
lfb_pic dd 0
