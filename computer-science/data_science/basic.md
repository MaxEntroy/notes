## Storage

### Object Storage

Object storage, also known as object-based storage, is a computer data storage architecture designed to handle large amounts of **unstructured data**. Unlike other architectures, **it designates data as distinct units**, bundled with metadata and a unique identifier that can be used to locate and access each data unit. 

These units—or objects—can be stored on-premises, but are typically stored in the cloud, making them easily accessible from anywhere.

Much of today’s data is unstructured: email, media and audio files, web pages, sensor data, and other types of digital content that do not fit easily into traditional databases. As a result, finding efficient and affordable ways to store and manage it has become problematic. Increasingly, object storage has become the preferred method for storing static content, data arches, and backups. 

背景，目前有很多无结构化数据，随着大数据的兴起。这部分数据的高效检索是一个挑战，因此如何存好这种特性的数据是os这种方式出现的动机。


下面看一下对比

#### File storage

File storage stores and organizes data into folders, similar to the physical files you might store in a paper filing system in an office. If you need information from a file, you’ll need to know what room, cabinet, drawer, and folder contains that specific document. This same hierarchical storage structure is used for file storage, where files are named, tagged with metadata, and then placed in folders. 

To locate a piece of data, you’ll need to know the correct path to find it. Over time, searching and retrieving data files can become time-consuming as the number of files grows. While scalability is more limited, it is a simple way to store small amounts of just about any type of data and make it accessible to multiple users at once. 

如上文所言，os这种形式出现的动机是为了寻找高效的数据查询方式。可以换个理解方式，有一个学校的学生，我们怎么组织他们，能让我们高效的存储。文件就是所谓的hier存储方式，划分一级一级的level进行数据存储。年级-班-小组-学生。因此，其查询效率需要知道完整路径。包括计算机硬盘的数据存储方式，都是以文件的形式存储。有点就是层次结构的优势，层次分明。层次代表着一定程度的优先级。

#### Object storage

Object storage, as discussed earlier, saves files in a **flat data environment**, or storage pool, as a self-contained object that contains all the data, a unique identifier, and detailed metadata that contains information about the data, permissions, policies, and other contingencies. **Object storage works best for static storage, especially for unstructured data, where you write data once but may need to read it many times.** 

While object storage eliminates the need for directories, folders, and other complex hierarchical organization, it’s not a good solution for dynamic data that is changing constantly as you’ll need to rewrite the entire object to modify it. In some cases, file storage and block storage may still suit your needs depending on your speed and performance requirements.  

适用特点说的很明白，单写多读。存储的时候，拍平了存储。存储数据有meta数据，方便查询。由于是flat存储，只适合静态存储，不要总是调整，后者变更频繁，是动态数据的范畴，不适合。

[What is Object storage?](https://cloud.google.com/learn/what-is-object-storage)