# Secure-Report
The Secure Retrieval Protocol, as defined in SECURE_PROTOCOL_H, implements a virtual API for securely retrieving data from a server using Fully Homomorphic Encryption (FHE) mechanisms. This protocol is tailored towards ensuring utmost privacy against potential server breaches while enabling the server to perform computations on the data obliviously.

## Project Description
<!-- Homomorphic Encryption -->
- **Homomorphic Encryption**: The protocol employs Fully Homomorphic Encryption using the HElib library, a notable library for homomorphic encryption. This type of encryption allows computations to be performed directly on the encrypted data, returning encrypted results which, when decrypted, match the results of the operations as if they were performed on the plain data.

<!-- Privacy and Security -->
- **Privacy and Security**: The primary aim of this protocol is to secure data against the server itself. By using FHE, the server can execute operations on the encrypted data without ever knowing the actual data. This level of security ensures that even if the server is compromised, the raw data remains protected.

<!-- Functionality -->
- **Functionality**: The protocol's main functionality is to fetch matches from a database based on client queries. The results are securely and efficiently retrieved without compromising the integrity and privacy of the original data.

<!-- Secure Batch Retrieval -->
- **Secure Batch Retrieval (SBR)**: The Secure Batch Retrieval mechanism is an advanced feature that not only allows batch retrievals but offers flexibility for the user. Users can specify the number of matches they wish to retrieve, offering a degree of customization based on user requirements.

## Key Components:

<!-- Secure Retrieval Protocol Class -->
1. **Secure Retrieval Protocol (SecureRetrievalProtocol class)**:
    - **Initialization**: It is designed to initialize entities such as the client, server, and trusted third party.
    - **Upload**: A client encrypts plain database data using FHE and uploads the encrypted data to the server.
    - **Query**: A client encrypts a plain query and sends the encrypted query to the server for processing.
    - **Retrieve**: An abstract method where the client retrieves output from the server. This output is evaluated based on the previously sent encrypted query.

<!-- Secure Batch Retrieval Protocol Class -->
2. **Secure Batch Retrieval Protocol (SecureBatchRetrievalProtocol class)**:
    - Inherits from the base SecureRetrievalProtocol. This class is designed for batch retrieval, meaning it can retrieve multiple sets of data in one go.
    - **Retrieve**: Involves retrieving data in batches. The number of batches is determined by the number of batches within the query.
    - **Retrieve Batch**: Retrieves a specific batch of data.

<!-- Secure Report Protocol Class -->
3. **Secure Report Protocol (SecureReportProtocol class)**:
    - This inherits from the base SecureRetrievalProtocol.
    - **Retrieve**: Similar to the method in SecureRetrievalProtocol, it retrieves data based on an encrypted query.

<!-- CONCLUSION -->
In essence, the Secure Retrieval Protocol provides a robust, secure, and flexible solution for data retrieval with a strong emphasis on user privacy, data integrity, and server-side security. Fully Homomorphic Encryption, with the backing of the HElib library, stands at the core of this protocol, offering advanced computational capabilities on encrypted data.

The Secure Batch Retrieval Protocol is designed to facilitate the secure transfer of sensitive data from a Server to a Client. It leverages Fully Homomorphic Encryption (FHE) to ensure that the Server processes only encrypted data, thus maintaining the privacy and integrity of the data throughout the retrieval process.

## Entities Involved
- **Client**: The entity that initiates the protocol to securely retrieve data from the server.

- **Server**: The entity that holds the encrypted data and processes encrypted queries.

## Protocol Sketch
### 1. **Initialization**
   
   - **Input**: Configuration settings, security parameters, and protocol initialization request from the Client.
   
   - **Output**: A secure communication channel established, encryption keys generated, and both entities prepared for secure data exchange.

### 2. **Upload**
   - **Input**: The Client's database data in plain format.
   - **Output**: Encrypted data is securely stored on the Server using Fully Homomorphic Encryption (FHE).

### 3. **Query**
   - **Input**: The Client's query in plain format, including search terms, filters, order instructions, and batch size.
   - **Output**: Encrypted query constructed using the Client's encryption keys, sent to the Server for processing.

### 4. **Batch Retrieval**
   - **Input**: The Server's encrypted query processing algorithm.
   - **Output**: Encrypted data batches that match the Client's query, sent back to the Client without revealing any data content to the Server.

### 5. **Decryption**   
   - **Input**: Encrypted data batches received from the Server.
   - **Output**: Decrypted data batches that the Client can read and analyze, converted back to plain format.

### 6. **Conclusion**   
   - **Outcome**: The Client acquires the requested data in decrypted form, ready for use, while the security and privacy of the data have been preserved throughout the process.

## Sequence Diagram
The diagram illustrates the step-by-step communication flow, specifying the nature of data exchanged (plain or encrypted) at each stage between the Client and the Server.

```plaintext
   Client               Server
     |                              |
     |                              |
     |    --- Initialization ---    |
     |                              |
     |                              |
     |--- Upload Encrypted Data --->|
     |                              |
     |   (Send Encrypted Queries)   |
     |-------Encrypted Query------->|
     |                              |
     | (Retrieve Encrypted Batches) |
     |<------Encrypted Batches------|
     |                              |
     | (Receive Decrypted Batches)  |
     |---Client Decrypted Batches   |
     |                              |
```

## Dependencies:
This project relies on the following libraries and tools:
- **pThread:** A POSIX threads library essential for enabling concurrent operations.
- **GMP**: A pivotal library for high-precision arithmetic operations.
- **NTL:** A C++ library facilitating comprehensive data structures and algorithms.
- **FHE & Liphe:** Libraries handling the complexities of fully homomorphic encryption.
- **Google Tests (gtest):** A testing framework ensuring the robustness and correctness of the codebase.
- **Eigen3:** A linear algebra C++ template library indispensable for matrix operations and related computations.
The project mandates the placement of dependencies' include files in directories: "/usr/include" or "/usr/local/include", and their library files in "/usr/lib" or "/usr/local/lib" for optimal functioning on Linux platforms.

### Technical Implementations and Dependencies
1. The present C++ implementation integrates the Eigen library for linear algebraic operations. Further details and documentation can be sourced from [Eigen's official page](http://eigen.tuxfamily.org/index.php?title=Main_Page).
2. A supplementary Python rendition of the Saffron algorithm is housed in the 'python_sketch' directory. An optimized version curated for benchmarking is hosted on [GitHub](https://github.com/gil7788/disjunct_benchmark).
