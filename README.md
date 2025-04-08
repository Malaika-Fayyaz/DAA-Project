# 🔐 RSA Key Generator using C and Cloud Computing

## 📘 Introduction

This project aims to build an efficient and secure **RSA Key Generator** using the **C programming language**, optionally optimized with **assembly**, and enhanced by the computational power of **cloud computing**. It is being developed as part of an academic course project.

---

## 💡 The Core Idea: Stringing it All Together

We started from the basic principle of RSA encryption—generate two large prime numbers and use them to compute a public-private key pair.

Basic flow:
- Generate two relatively large prime numbers
- Multiply them to get the modulus `n`
- Compute Euler's totient `phi(n)`
- Choose a standard public exponent `e` (commonly 65537)
- Compute the private key `d` such that `d * e ≡ 1 (mod phi(n))`

### Why C?
Chosen for its efficiency and low-level control, making it ideal for cryptographic computations.

### Why the Cloud?
Generating large primes can be computationally expensive. Cloud computing provides scalable resources to perform these operations quickly.

### Assembly Use?
For performance-critical components like modular exponentiation, assembly-level optimization might be integrated for speed gains.

This brings together **low-level efficiency** and **scalable computing power** to build a capable RSA key generation system.

---

## 🚀 Applications

- 🔐 **Secure Communication** – Used in encrypted messaging and secure protocols
- 📚 **Education** – Great for understanding cryptography and system-level programming
- 🛠️ **Practical Tool** – Can extend to secure file transfer, digital signatures, etc.

---

## ☁️ Cloud Use Case

- **Platform Options**: AWS EC2, Google Cloud Compute Engine, or Azure VMs
- **Why Cloud?**
  - Faster generation of large primes
  - Scalable and distributed computation
  - Flexibility for future extensions

The C application will be deployed on a cloud VM with sufficient CPU/memory. It handles key generation server-side and optionally returns the output to a client machine. For advanced optimization, we may explore parallel prime generation using tools like **MPI**.

---

## 🛠️ Implementation Overview

The RSA generator will be compiled and deployed to a cloud-based virtual machine. A startup script sets up the environment and installs necessary dependencies. The C program runs server-side to generate large primes and compute RSA keys.

The resulting keys can be displayed or downloaded. Future additions may include:
- A CLI or Web UI
- Cloud-native APIs for requesting keys
- Parallel execution models
- Benchmarking reports

---

## ✅ Conclusion

This project brings together system-level programming and cloud computing to create a secure, fast RSA key generation system. It will evolve with new features, optimizations, and learning as we progress.
