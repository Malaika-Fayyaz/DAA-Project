Sure! Here's your project overview rewritten in a well-structured **README.md** format:

---

# RSA Key Generator using C and Cloud Computing

## Overview
This project implements a secure and efficient RSA key generator using the C programming language, optionally optimized with assembly for performance. It leverages **cloud computing** to distribute the load of **large prime number generation**, using a **multithreaded client** and **distributed servers**. This approach ensures speed, scalability, and robustness for cryptographic key generation.

---

## Core Idea

The RSA encryption algorithm relies on the following steps:

1. Generate two large prime numbers `p` and `q`.
2. Compute the modulus: `n = p * q`.
3. Compute Euler’s totient: `phi(n) = (p - 1)(q - 1)`.
4. Choose a standard public exponent: `e = 65537`.
5. Compute the private key: `d` such that `d * e ≡ 1 (mod phi(n))`.

---

## Why C?

- Low-level control
- High computational efficiency
- Ideal for cryptographic algorithms

---

## Why Cloud?

- Prime generation is CPU-intensive
- Cloud provides scalability and speed
- Enables future expansion to multi-client systems

---

## Tools & Technologies

- **C Programming**
- **Miller-Rabin Primality Test** (probabilistic method)
- **pthreads** for multithreaded connections
- **Winsock (local)** / **Unix Sockets (cloud)**
- **Microsoft Azure** – Ubuntu Server 22.04 LTS VM

---

## System Architecture

### Server Side (`server1.c`, `server2.c`)
- Listens on ports `9999` and `9998`
- Randomly generates 5–6 digit odd numbers
- Applies the Miller-Rabin test for primality
- Sends a valid prime to the client

### Client Side (`client.c`)
- Spawns **two threads** to connect to both servers
- Receives two primes: `p` and `q`
- Computes:
  - `n = p * q`
  - `phi = (p - 1) * (q - 1)`
  - `e = 65537`
  - `d = modular inverse of e mod phi`
- Outputs **RSA public and private keys**

---

## Applications

- **Educational Tool**: Learn cryptography & low-level programming
- **Secure Communication**: Foundation for encryption protocols
- **Practical Use**: Encrypted messaging, secure file sharing, etc.

---

## Cloud Deployment

### Cloud Platform:
- **Microsoft Azure**
- **VM**: Ubuntu Server 22.04 LTS (Standard_B1s, free-tier eligible)

### Why Cloud?
- Faster prime generation
- Scalable resource allocation
- Supports future features (web UI, parallel clients, etc.)

---

## Project Milestones

### Milestone 1: 20% – Initial Setup
- Designed a local client-server model using **Winsock**
- Used **trial division** for primality testing
- Verified full RSA key generation flow locally

### Milestone 2: 60% – Cloud Transition & Optimization
- Replaced trial division with **Miller-Rabin**
- Switched to **Unix sockets** for cloud deployment
- Set up **Azure VM**
- Deployed servers (`server1`, `server2`) on ports 9999 & 9998
- Used **pthreads** for concurrent client connections
- Client retrieved primes and computed keys successfully

---

## Final Milestone: 100% – Planned Enhancements

- **Deploy client.c to the cloud** to complete cloud-based RSA generation
- **Build CLI or Web UI** for user interaction
- **Generate longer primes** (300–600 digits) for 1024/2048/4096-bit RSA
- **Secure socket communication** (TLS or encryption)
- **Implement error handling & logging**
- **Benchmark** prime generation and key computation across VM sizes

---

## Future Potential

- Parallel prime generation with **MPI**
- Real-world scale RSA (4096-bit keys)
- Lightweight API or encryption microservice
- Integration with secure messaging or blockchain

---

## Conclusion

This project showcases how to build a practical, distributed RSA key generator using low-level programming and the power of cloud computing. It blends:
- Algorithmic knowledge (RSA, primality tests)
- System programming (sockets, multithreading)
- Modern cloud practices (scalable deployment)

Whether you're exploring cryptography, optimizing performance, or learning distributed systems, this project serves as a strong foundation.
