# 👀 Problem

> vector<State> 出现错误
> 

# 💭 Answer

> 
> 

---

# 👀 Problem

> **vector的 clear()函数**
> 

# 💭 Answer

> clear（）函数的调用方式是，vector temp（50）;//定义了50个datatype大小的空间。temp.clear();
> 
> 
> **作用**：**将会清空temp中的所有元素，包括temp开辟的空间（size），但是capacity会保留**，即不可以以temp[1]这种形式赋初值，只能通过temp.push_back(value)的形式赋初值。
> 
> 同样对于vector<vector > temp1（50）这种类型的变量，使用temp1.clear()之后将会不能用temp1[1].push_back(value)进行赋初值，只能使用temp1.push_back(temp)；的形式。
> ————————————————
> https://blog.csdn.net/ZauberC/article/details/128042048
> 

---

# 👀 Problem

> **output.txt出现奇怪的符号 e.g. ^M, ^@**
> 

# 💭 Answer

> write 的长度大于想要输出的长度
> 

注意：getline 会包含换行符

---

# 👀 Problem

> **output.txt出现奇怪的符号 e.g. ^M, ^@**
> 

# 💭 Answer

> write 的长度大于想要输出的长度
> 

注意：getline 会包含换行符
