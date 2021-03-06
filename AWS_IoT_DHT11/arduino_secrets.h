// Fill in  your WiFi networks SSID and password

#define SECRET_SSID "지100x100"
#define SECRET_PASS "123456789"

// Fill in the hostname of your AWS IoT broker
#define SECRET_BROKER "a1fy9pcff2h0c5-ats.iot.ap-northeast-2.amazonaws.com"

// Fill in the boards public certificate
const char SECRET_CERTIFICATE[] = R"(
-----BEGIN CERTIFICATE-----
MIICiTCCAXGgAwIBAgIVALh91bPpa30zDgA2LraAcgE5SXuVMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTEwMjYwODIz
MDJaFw00OTEyMzEyMzU5NTlaMBgxFjAUBgNVBAMTDU15TUtSV2lGaTEwMTAwWTAT
BgcqhkjOPQIBBggqhkjOPQMBBwNCAAR0nnb88+F7U5jgNTcgk1W2y1OnkDU/T6qL
RWIeR1dM5fYViCjnhnjOs8mMbeysk5rEPmynzOKyPISloqFS42s6o2AwXjAfBgNV
HSMEGDAWgBR7QUBTnBkwIrubFAC2TCVuCjZbrzAdBgNVHQ4EFgQUOawJFQUQo9tQ
YHZnzo4/0WUKzIcwDAYDVR0TAQH/BAIwADAOBgNVHQ8BAf8EBAMCB4AwDQYJKoZI
hvcNAQELBQADggEBADVmvWoxiHpCY67cy5pAfCHs8SqaOViFvA1cdF++zlT+HvZc
M0yBodT+6CT2klyH+kNKT5levLi6gwa9X+kZ5vzfK0lBTP3MpZJha7t+BHHkzooQ
8RPwCd8beInc1t6UoR6gJWTyLqyv7p40e98pkDLBsqANqyigdaTkvhUyBMCvQtMi
9vLNSz6L26ECCLt+wcSrQIRYsMt8v9cqsIHEX/EraI8Ji4bDa7VvbDVS/WcQhecB
hpDuhWg1iU/I0p+PceoFoDRarjn4AU5Ep1HNmKg8/REHYP3WhT1HaBtyNeM0d05O
2ZsfFoE0Ams/qVVUtkcWwp7wrlJdv00Ul+Txn74=
-----END CERTIFICATE-----
)";
