#!/usr/bin/python3 -u

def encrypt(message):
    import rsa
    pub_key_str=b'-----BEGIN RSA PUBLIC KEY-----\nMIIBCgKCAQEAjELIz6I/+4X4zNp4M0vHuQQVWKbsHC5FdepzeD3Dzt0VjWcLb89D\n/B06FfJ4NrLIkVVoe7lnqW0GRSYsUH2+OJhcFg7xv5CUOBF+f6ERiXDQabo3Ico4\nbNn57Wn/Am22v1cpUfZr1r1P3RoamkoMHEWJGcYg8iTAONe0NF04/hf2oSzMxwK2\nnwDodvqg7h3ImZ3GUL4GBUDtluwGDWPoDPgkwW2pGmGLAQnTZoy+6zTiw97VE7BG\nu3wmRhNESR4d2i4rw1I2UZ4bP/d6JHiz0iTHZahdrpKHSiBObDZmBJjrQdpMWllz\n4IZsKdIUWlHKWoyon7jCmgcM2N7jEwmHrQIDAQAB\n-----END RSA PUBLIC KEY-----\n'
    pub_key = rsa.PublicKey.load_pkcs1(pub_key_str)
    return rsa.encrypt(message.encode(), pub_key).hex()

if __name__ == '__main__':
    message = '{}'.format(75.343)
    encrypted = encrypt(message)

    print('F1={}'.format(encrypted))



