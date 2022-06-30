### Readme_第八次作业说明

​	以下是我对原有的潜艇大战游戏的改进说明。具体运行效果见附件 “潜艇大战 - 改进效果动画.mp4”。

##### 1.增加了炸弹种类

​	采取添加类的方式，在炸弹类Bomb的基础上增加了升级炸弹类Bplus，更换了炸弹贴图，同时新增了触发发射升级炸弹的按键。

​	详情见文件Bplus.h以及Bplus.cpp。

##### 2.增加了音效

###### 	1）爆炸音效

​	当我方舰艇发射的炸弹击中敌方潜艇时，会在绘制出爆炸贴图之后触发爆炸音效；敌方潜艇发射的鱼雷在击中我方舰艇时，也会产生同样的效果。

###### 	2）游戏结束音效

​	当我方舰艇的生命值耗尽时，会触发游戏结束音效。

##### 3.增加了舰艇寿命

​	在原有的代码中，我方舰艇只要被敌方鱼雷击中，游戏便会结束；我通过增加一个全局变量life表示舰艇的生命值，初始值为30，每次被鱼雷击中减少10，当生命值小于等于0时，游戏才会结束，从而增加了舰艇的寿命。

( SubMarineView.h 文件)

```c++
	// 船只的生命值
	int life;
```

( SubMarineView.cpp 文件)

```c++
	life = 30 ;
```

```c++
					//减少船只的生命值
					life -= 10;
					// 删除我方船只
					if (life <= 0) {
						m_Objects[MYSHIP].RemoveAt(pos4);
						delete pMyShip;
						//增加结束音效
						PlaySound(LPCSTR(IDR_OVER), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					}
```



​	以下是在Microsoft Docs查阅到的对 CObList 与 CImageList 两个类的说明：

##### 	1. CObList

> ## 备注
>
> `CObList` 列表的行为类似于双向链接列表。
>
> 类型为 `POSITION` 的变量是列表的键。 可以使用 `POSITION` 变量作为迭代器来按顺序遍历列表，并将其作为书签来保存位置。 但位置不同于索引。
>
> 元素插入速度非常快，在列表头、尾部和已知 `POSITION` 的位置。 需要顺序搜索按值或索引查找元素。 如果列表很长，则此搜索可能会很慢。
>
> `CObList` 包括用于支持其元素序列化和转储的 `IMPLEMENT_SERIAL` 宏。 如果使用重载的插入运算符或 `Serialize` 成员函数将指针列表 `CObject` 存储到存档中，则将依次序列化每个 `CObject` 元素。
>
> 如果需要转储列表中的各个 `CObject` 元素，则必须将转储上下文的深度设置为1或更大。
>
> `CObList`删除对象时，或删除其元素时，只 `CObject` 会删除指针，而不会删除它们引用的对象。
>
> 您可以从派生您自己的 `CObList` 类。 新的列表类，旨在保存指向派生自 `CObject` 的对象的指针，并添加新的数据成员和新的成员函数。 请注意，结果列表并不是严格类型安全的，因为它允许插入任何 `CObject` 指针。
>
> ### 公共方法
>
> |                            “属性”                            |                             说明                             |
> | :----------------------------------------------------------: | :----------------------------------------------------------: |
> | [`CObList::AddHead`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#addhead) | 将一个元素 (或另一个列表中的所有元素) 添加到列表的开头 (会成为新的 head) 。 |
> | [`CObList::AddTail`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#addtail) | 将一个元素 (或另一个列表中的所有元素) 添加到列表的尾部 (会生成新的尾部) 。 |
> | [`CObList::Find`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#find) |                获取由指针值指定的元素的位置。                |
> | [`CObList::FindIndex`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#findindex) |             获取以零为基的索引指定的元素的位置。             |
> | [`CObList::GetAt`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#getat) |                    获取给定位置处的元素。                    |
> | [`CObList::GetCount`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#getcount) |                    返回此列表中的元素数。                    |
> | [`CObList::GetHead`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#gethead) |                返回 (不能为空) 列表的头元素。                |
> | [`CObList::GetHeadPosition`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#getheadposition) |                    返回列表头元素的位置。                    |
> | [`CObList::GetNext`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#getnext) |                获取用于循环访问的下一个元素。                |
> | [`CObList::GetPrev`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#getprev) |                获取用于循环访问的上一个元素。                |
> | [`CObList::GetSize`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#getsize) |                    返回此列表中的元素数。                    |
> | [`CObList::GetTail`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#gettail) |               返回列表的尾元素， (不能为空) 。               |
> | [`CObList::GetTailPosition`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#gettailposition) |                   返回列表的尾元素的位置。                   |
> | [`CObList::InsertAfter`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#insertafter) |                 将新元素插入到给定位置之后。                 |
> | [`CObList::InsertBefore`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#insertbefore) |                 将新元素插入到给定位置之前。                 |
> | [`CObList::IsEmpty`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#isempty) |                测试空列表条件 (不) 任何元素。                |
> | [`CObList::RemoveAll`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#removeall) |                   从此列表中移除所有元素。                   |
> | [`CObList::RemoveAt`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#removeat) |               从此列表中移除按位置指定的元素。               |
> | [`CObList::RemoveHead`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#removehead) |                     从列表头中删除元素。                     |
> | [`CObList::RemoveTail`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#removetail) |                    从列表的末尾移除元素。                    |
> | [`CObList::SetAt`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/coblist-class?view=msvc-170#setat) |                   设置位于给定位置的元素。                   |

##### 	2. CImageList

> ## 备注
>
> "图像列表" 是大小相同的图像的集合，每个图像都可以通过其从零开始的索引来引用。 图像列表用于有效地管理大的图标或位图集。 图像列表中的所有图像都包含在屏幕设备格式的单个宽位图中。 图像列表可能包含一个单色位图，该位图包含用于透明地绘制图像的蒙板（图标样式）。 Microsoft Win32 应用程序编程接口 (API) 提供了图像列表功能，使你能够绘制图像、创建和销毁图像列表、添加和删除图像、替换图像、合并图像和拖动图像。
>
> 此控件 (，因此 `CImageList` 类) 仅适用于在 95/98 Windows Windows NT 和更高版本3.51 及更高版本下运行的程序。
>
> ### 公共方法
>
> |                            “属性”                            |                             说明                             |
> | :----------------------------------------------------------: | :----------------------------------------------------------: |
> | [`CImageList::Add`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#add) |                  向图像列表添加图像或图像。                  |
> | [`CImageList::Attach`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#attach) |             将图像列表附加到 `CImageList` 对象。             |
> | [`CImageList::BeginDrag`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#begindrag) |                        开始拖动图像。                        |
> | [`CImageList::Copy`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#copy) |               复制对象中 `CImageList` 的图像。               |
> | [`CImageList::Create`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#create) |       初始化图像列表，并将其附加到 `CImageList` 对象。       |
> | [`CImageList::DeleteImageList`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#deleteimagelist) |                        删除图像列表。                        |
> | [`CImageList::DeleteTempMap`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#deletetempmap) | 由 [`CWinApp`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cwinapp-class?view=msvc-170) 空闲时间处理程序调用以删除由创建的 `FromHandle` 任何临时 `CImageList` 对象。 |
> | [`CImageList::Detach`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#detach) |  从 `CImageList` 对象分离图像列表对象并返回图像列表的句柄。  |
> | [`CImageList::DragEnter`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#dragenter) |     在拖动操作过程中锁定更新，并在指定位置显示拖动图像。     |
> | [`CImageList::DragLeave`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#dragleave) |          解锁窗口并隐藏拖动图像，以便可以更新窗口。          |
> | [`CImageList::DragMove`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#dragmove) |             在拖放操作过程中移动正在拖动的图像。             |
> | [`CImageList::DragShowNolock`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#dragshownolock) |       在拖动操作期间显示或隐藏拖动图像，而不锁定窗口。       |
> | [`CImageList::Draw`](https://docs.microsoft.com/zh-cn/cpp/mfc/reference/cimagelist-class?view=msvc-170#draw) |              绘制拖放操作过程中正在拖动的图像。              |

