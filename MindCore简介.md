# MindCore简介

### 简介

​	面向目标型AI，通过设置目标优先级和以及行为来实现AI行为控制

### 使用方法

​	将项目导入Plugins，将Mind组件挂载到Actor上，设置相关的配置及行为即可

### 组成

#### 	Mind

​		Actor需要绑定的组件，使用外观模式的设计，玩家所需要做的答题配置可以在Mind上进行设置

#### 	Wish

​		用于管理AI目标，如字面意思。

#### 	Behavior

​		对AI行为的规划管理，管理着具体的MindAction。

### 	MindAction

​		行为的最小模块，可以通过这个组件设置具体的行为。

#### 	Remember

​		用于AI行为的存储管理。

#### 	Image

​		如字面意思，全局的控制行为。