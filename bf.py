bfcmd = input('String: ')
cells = [0]
curcell = 0
loopstack = [0]
usedloops = []


# i is index of character char is the character
while True:
    #print("loopstack: ", loopstack)
    #print("starting at: ", loopstack[-1])
    for i, char in enumerate(bfcmd):
        #print(f"{cells[curcell]}:{curcell}")
        #print(char)
        if i < loopstack[-1]:
            continue
        else:
            match char:
                case '+':
                    cells[curcell] += 1
                    if cells[curcell] == 256:
                        cells[curcell] = 0

                case '-':
                    cells[curcell] -= 1
                    if cells[curcell] == -1:
                        cells[curcell] = 255

                case '<':
                    curcell -= 1
                    if curcell < 0:
                        print("current cell is less than 0")

                case '>':
                    curcell += 1
                    if curcell == len(cells):
                        cells.append(0)

                case '.':
                    print(chr(cells[curcell]), end='')

                case ',':
                    pass

                case '[':
                    loopstack.append(i)
                    usedloops.append(i)
                    #print("num added to loopstack", str(i))

                case ']':
                    if cells[curcell] == 0:
                        loopstack.pop()
                    else:
                        break

                case _:
                    print("ruh roh scoob")
                    break
                
    else:
        break