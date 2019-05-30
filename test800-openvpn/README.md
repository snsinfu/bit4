```
/srv/
│
├─ certs/
│  │
│  └─ pki/
│     │
│     ├─ issued/
│     │  ├─ server.crt              Server certificate
│     │  ├─ client-alice.crt        Client certificate
│     │  ├─ client-bob.crt          .
│     │  └─ ...
│     │
│     ├─ private/
│     │  ├─ server.key              Server private key
│     │  ├─ client-alice.key        Client private key
│     │  ├─ client-bob.key          .
│     │  └─ ...
│     │
│     ├─ dh.pem                     Server DH parameters
│     ├─ ca.crt                     CA certificate
│     └─ crl.pem                    CA CRL
│
└─ vpn
   │
   ├─ clients/
   │  ├─ client.ovpn                Client config template
   │  ├─ client-alice.ovpn          Client config
   │  ├─ client-bob.ovpn            .
   │  └─ ...
   │
   ├─ openvpn.conf                  Server config
   └─ tls_auth.pem                  TLS-Auth PSK
```
