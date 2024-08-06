# Owen Kroeger
# My Own Work
# Program 3, Vigenere Cipher


plaintext = ""
key = ""
newkey = ""


# Function Receives inputs from user
def receiveInputs():

    ptInput = input("Enter the text to encode: ")

    global plaintext
    plaintext = ptInput
    
    keyInput = input("Enter a key for the cypher: ")
    
    print()

    global key
    key = keyInput

    print("Original text...")
    print(plaintext)
    print()


# Takes the key from the user, and returns the key 
# spread out to the length of the string to be 
# decoded
def genKey():
    
    # length of key matches text, no need to calculate further
    if(len(key) == len(plaintext)):
        return key

    # else key is shorter than text
    else:
        newkey = ""
        temp = []
        count = 0

        # for i in range length of plaintext
        for i in plaintext:

            # create new key one letter at a time
            temp.append(key[count])

            # if reached end of key, go back to beginning
            if((count + 1) >= len(key)):
                count = 0

            # else continue through key
            else:
                count += 1

        # join array into string
        newkey = ''.join(str(x) for x in temp)

        return newkey


# takes the inputted string and key generated, and
# encodes via Vigenere Cipher according to the key
def encode(text, code):

    encoded = []
    tempchar = ''

    # generate the string based on key
    # for i in range length of input string
    for i in range(len(text)):

        # if the character is special character
        # don't change it (space, apostraphe, etc.)
        if(ord(text[i]) < 97):
            tempint = ord(text[i])

        # else, encode it based on key
        else:

            # add ascii values of text char and key char
            # mod result by 26 for new encoded char
            # must add the base of 97 (ascii of 'a') to generate 
            # the correct character
            tempint = (ord(text[i]) + ord(code[i])) % 26
            tempint += ord('a')

        # convert ascii to char, add to encoded array
        tempchar = chr(tempint)
        encoded.append(tempchar)

    # join encoded array into string
    encoded = ''.join(str(x) for x in encoded)

    # print encoded string
    print("Encoded string...")
    print(encoded)
    print()

    return encoded

#encode(plaintext, genKey())

# takes the encoded string and key generated, and
# decodes via Vigenere Cipher according to the key
def decode(text, code):

    decoded = []
    tempchar = ''

    # for i in range length of input string
    for i in range(len(text)):

        # if char is special char, don't change it
        if(ord(text[i]) < 97):
            tempint = ord(text[i])

        # else, decode according to key
        else:

            # char of input str - char of key + 14 (based on ascii value diff)
            # mod result by 26 to get original (decoded) char
            # must add base of 97 (ascii of 'a') to generate correct char
            tempint = (ord(text[i]) - ord(code[i]) + 14) % 26
            tempint += ord('a')

        # convert ascii into char, store in array decoded
        tempchar = chr(tempint)
        decoded.append(tempchar)

    # join decoded array into string and print decoded string
    decoded = ''.join(str(x) for x in decoded)

    print("Decoded string... ")
    print(decoded)
    print()

    return decoded



receiveInputs()

genKey()

decode(encode(plaintext, genKey()), genKey())
