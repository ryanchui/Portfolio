#!/usr/bin/env python
# coding: utf-8

# In[1]:


import pandas as pd
import numpy as np
import os
import matplotlib.pyplot as plt
import numpy as np
import cv2
import PIL
import tensorflow as tf
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from datetime import datetime

from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans


# In[2]:


customers = pd.read_csv ('~/Downloads/wetransfer_sia-partners-take-home-case-study_2023-04-28_0704/dataset/customers_dataset.csv')
order = pd.read_csv ('~/Downloads/wetransfer_sia-partners-take-home-case-study_2023-04-28_0704/dataset/orders_dataset.csv')
order_items = pd.read_csv ('~/Downloads/wetransfer_sia-partners-take-home-case-study_2023-04-28_0704/dataset/order_items_dataset.csv')
products = pd.read_csv ('~/Downloads/wetransfer_sia-partners-take-home-case-study_2023-04-28_0704/dataset/products_dataset.csv')
products_category = pd.read_csv ('~/Downloads/wetransfer_sia-partners-take-home-case-study_2023-04-28_0704/dataset/product_category_name_translation.csv')


# In[3]:


df = pd.merge(customers, order, on="customer_id", how="outer")
df2  = pd.merge(products, products_category, on="product_category_name", how="outer")
final = order_items.merge(df,how ='left', on = "order_id").merge(df2,how ='left', on = "product_id")
final


# Data Cleaning step to ensure all labels are assigned to a value instead of na value

# In[4]:


# plt.hist(final['product_weight_g'])
# print(final['product_weight_g'].mean())
# plt.hist(final['product_weight_g'])
# print(final['product_weight_g'].mean())
# plt.hist(final['product_width_cm'])
# print(final['product_width_cm'].mean())

# Replace nan with median since there are outliers and is more recommendable to use the median compared to mean.
final['product_weight_g'].fillna(final['product_weight_g'].median(),inplace=True)
final['product_length_cm'].fillna(final['product_length_cm'].median(),inplace=True)
final['product_height_cm'].fillna(final['product_height_cm'].median(),inplace=True)
final['product_width_cm'].fillna(final['product_width_cm'].median(),inplace=True)
final['product_photos_qty'].fillna(final['product_photos_qty'].median(),inplace=True)
final['product_description_lenght'].fillna(final['product_description_lenght'].median(),inplace=True)
final['product_name_lenght'].fillna(final['product_name_lenght'].median(),inplace=True)

# final['order_approved_at'] = final['order_approved_at'].fillna('')
# final['order_delivered_carrier_date'] = final['order_delivered_carrier_date'].fillna('')
# final['order_delivered_customer_date'] = final['order_delivered_customer_date'].fillna('')
# final.isnull().sum()


# In[5]:


## Double check no missing value is assigned in product_width_cm, product_length_cm, product_height_cm, product_width_cm 
final.isnull().sum()
## No duplicate
final[final.duplicated()]


# In[6]:


delivered = final.loc[final['order_status'] == 'delivered']
result = delivered['product_category_name_english'].value_counts().nlargest(10)

## Return as series and print first 10.
result.plot(kind='bar', title='Top Delivered products by category', ylabel = 'count', xlabel = 'Product categories')
result[:10]


# Total Delivered Orders by Product Category: Bed_Bath_Table is the most ordered product by product category, having a total order of 10953, followed by Health_Beauty 9465 orders.

# In[7]:


## Group by product_category_name_english column, after .agg means I'm aggrating and work on these column and performing the sum
# Return as a data frame
# Crete a group by object for the product_category_name_english and applying aggregation function using sum
# Aggregate using one or more operations over the specified axis.
revenue = delivered.groupby(by=["product_category_name_english"]).agg({'price':'sum'})
# sort by price return as dataframe
revenue = revenue.sort_values(by='price', ascending=False)

revenue[:10].plot(kind='bar', title = 'Top 10 Products', ylabel = 'Total Revenue ($M)', xlabel = 'Product Category')
# Reset the index of the DataFrame, and use the default dataframe instead, so basically remove one level. need to restore the default numeric index
revenue[:10].reset_index()


# In[8]:


# returns all unique elements of a column for order_status
print(final['order_status'].unique())
print('delivered : ', len(final.loc[final['order_status'] == 'delivered']), 'delivered rate: ', len(final.loc[final['order_status'] == 'delivered'])/ len(final['order_status']))
print('shipped : ', len(final.loc[final['order_status'] == 'shipped']))
print('canceled : ', len(final.loc[final['order_status'] == 'canceled']), 'canceled rate: ', len(final.loc[final['order_status'] == 'canceled'])/ len(final['order_status']))
print('invoiced : ', len(final.loc[final['order_status'] == 'invoiced']))
print('processing : ', len(final.loc[final['order_status'] == 'processing']))
print('approved : ', len(final.loc[final['order_status'] == 'approved']))
print('unavailable : ', len(final.loc[final['order_status'] == 'unavailable']))

canceled = final.loc[final['order_status'] == 'canceled']
result = canceled['product_category_name_english'].value_counts().nlargest(10)
result.plot(kind='bar', title='Top cancel products by category', ylabel = 'Count')
result[:10]


# sports_leisure is the most product category being canceled, having a total order of 51, followed by housewares of 49 orders

# In[9]:


# returns all unique elements of a column for custom_id and customer_unique_id
print("Total number of customers are", len(delivered['customer_id'].unique()))
print("Total number of transaction made:", len(delivered['customer_unique_id'].unique()))
delivered.to_csv("delivered.csv")


# In[10]:


## https://www.geeksforgeeks.org/python-pandas-series-dt-day/
# need this otherwise, A value is trying to be set on a copy of a slice from a DataFrame.
# making sure Pandas know I intended to make it a copy rather than a view, so I make a copy of the dataframe
delivered = delivered.copy()

# parse to_datetime with parameter errors='coerce' and then use strftime for converting to weekday as locale’s full name
# print(df.strftime('%d-%b-%Y')) #01-Jun-2021
# print(df.strftime('%B')) #June
#  order_purchase_timestamp_month is changed to datetime type.
# https://stackoverflow.com/questions/36692861/avoiding-error-from-pd-to-datetime-in-pandas, https://www.plus2net.com/python/pandas-dt-to_datetime.php
# FIrst change order_purchase_timestamp column to datetime, use coerce because If our column has blank data ( specially when reading from Excel file ) and we want to continue with rest of the data then I should use coerce. 
# If errors = 'ignore' I may not able to convert to datatime format for all the rows
delivered['order_purchase_timestamp_month'] = pd.to_datetime(delivered['order_purchase_timestamp'], errors='coerce')
# get a list of directives to create formatted output by using strftime()
delivered['order_purchase_timestamp_month'] = delivered['order_purchase_timestamp_month'].dt.strftime('%Y/%m')

delivered['order_purchase_timestamp_daily'] = pd.to_datetime(delivered['order_purchase_timestamp'], errors='coerce')
delivered['order_purchase_timestamp_daily'] = delivered['order_purchase_timestamp_daily'].dt.strftime('%Y-%m-%d')



# In[11]:


monthly_order = delivered.groupby(by = ['order_purchase_timestamp_month']).agg({'order_status':'count'})
# monthly_order
fig, ax = plt.subplots(figsize=(12, 6))
monthly_order.plot(legend=False, title='Monthly delivered orders from 2016 - 2018', ax=ax)
ax.set_ylabel("Orders")
ax.set_xlabel('Purchase Time (Year - Month)')
plt.ylim([0, 9000])


# Highest transaction ofer occurs in the month of Nov 2017 with 8475 orders being placed, and the lowest transaction price occurs in the month Dec 2016 with only 1 order being placed.

# In[12]:


daily_order = delivered.groupby(by = ['order_purchase_timestamp_daily']).agg({'order_status':'count'})
daily_order = daily_order.sort_values(by='order_purchase_timestamp_daily', ascending=True).reset_index()

fig, ax = plt.subplots(figsize=(12, 6))
daily_order.plot(legend=False, title='Daily delivered sales orders from 2016 - 2018', ax=ax)
ax.set_ylabel("Orders")
ax.set_xlabel('Purchase Date')
plt.ylim([0, 1500])


# In[13]:


city_order = delivered.groupby(by = ['customer_city']).agg({'order_status':'count'})
# city_order2 = delivered.groupby(by = ['customer_city']).agg({'price':'sum'})
# city_order2.sort_values(by='price', ascending=False).head(15)

city_order = city_order.sort_values(by='order_status', ascending=False).head(15)
print(city_order)
city_order.plot(kind='bar', title='Top cities purchased from customers ', ylabel = 'Count', xlabel = 'Cities')


# Most customers were coming from Sao Paulo and spent $1859556.83 worth of products.

# In[14]:


cust_state = delivered.groupby(by = ['customer_state']).agg({'customer_id':'count'})
cust_state = cust_state.sort_values(by='customer_id', ascending=False).reset_index().head(10)
print(cust_state)

fig, ax = plt.subplots(figsize=(8, 4))
sns.barplot(x = cust_state['customer_state'], y = cust_state['customer_id'])
plt.title('Customer Frequency by State')
ax.set_xlabel('States')
ax.set_ylabel('Number of Customers')
plt.ylim([0, 50000])


# Most customers purchased from SP, followed by RJ and MG.

# In[15]:


print("Total number of unique seller_id:",len((delivered['seller_id']).unique()))
print("Total number of unique order_item_id:",len((delivered['order_item_id']).unique()))
print("Total number of unique customer_unique_id:",len((delivered['customer_unique_id']).unique()))
print("Total number of unique customer_id:",len((delivered['customer_id']).unique()))

seller = delivered.groupby(by = ['seller_id']).agg({'order_item_id':'count', 'price':'sum'}).reset_index()
seller = seller.sort_values(by='order_item_id', ascending=False).head(10)
# print('Top 5 seller unique ID', cust_state)
print(seller)

## Use below website as reference for below code
## https://towardsdatascience.com/creating-a-dual-axis-combo-chart-in-python-52624b187834
import matplotlib.patches as mpatches
# plot line graph on axis #1
# plot bar chart on axis #2
ax1 = sns.barplot(
    x=seller['seller_id'], 
    y='order_item_id', 
    data=seller, 
    color='orange', 
    alpha=0.5, 

)
ax1.set_xticklabels(ax1.get_xticklabels(), rotation=90)
ax1.set_ylabel('Number of orders')
ax1.set_xlabel('Seller ID')
ax1.set_ylim(0, 2500)
ax1_patch = mpatches.Patch(color='orange', label='Number of orders')
ax1.legend(handles=[ax1_patch], loc="lower right")


# # set up the 2nd axis
ax2 = ax1.twinx()
ax2 = sns.lineplot(
    x=seller['seller_id'], 
    y='price', 
    data=seller, 
    sort=False, 
    color='blue',
    ax = ax2       # Pre-existing axes for the plot
)
ax2.set_ylabel('Price')
ax2.set_ylim(0, 200000)
ax2_patch = mpatches.Patch(color='blue', label='Price')
ax2.legend(handles=[ax2_patch], loc="upper right")


# In[16]:


plt.figure(figsize=(15,8))
delivered['seller_id'].value_counts()[:10].plot.pie(autopct='%1.1f%%', shadow=True, startangle=90, cmap='tab20')
plt.title("Top 10 Sellers sold the most products",size=14, weight='bold')
plt.ylabel('')


# In[17]:


list(delivered.columns)

## only show numeric value, all string are excluded
df = delivered[['order_item_id',
 'price', 'freight_value',
 'customer_zip_code_prefix',
 'product_name_lenght',
 'product_description_lenght',
 'product_photos_qty',
 'product_weight_g',
 'product_length_cm',
 'product_height_cm',
 'product_width_cm']]

fig, ax = plt.subplots(figsize=(8,8))
sns.heatmap(df.corr(), annot=True, ax=ax, fmt=".2f")


# A slight positive correlation between product weight and freight, production weight and product height.

# In[18]:


# delivered.columns


# In[19]:


cust_purchase = final[[ 'customer_unique_id', 'order_purchase_timestamp']].copy()

cust_purchase = cust_purchase.groupby('customer_unique_id').agg({'order_purchase_timestamp':'max'}).reset_index()
cust_purchase
# cust_purchase.columns = ['customer_unique_id', 'last_purchase_timestamp']

cust_purchase = cust_purchase.rename(columns={'order_purchase_timestamp': 'last_purchase_timestamp'}) 

cust_purchase['last_purchase_timestamp'] = pd.to_datetime(cust_purchase['last_purchase_timestamp'], errors='coerce')

# create new variable active days to calculaute customers' last purchase
cust_purchase['Number of active days'] = (cust_purchase['last_purchase_timestamp'].max() - cust_purchase['last_purchase_timestamp']).dt.days
cust_purchase = cust_purchase[['customer_unique_id', 'Number of active days']]

print("active users", len(cust_purchase.loc[cust_purchase['Number of active days'] > 365]))
print("active users within one year" , len(cust_purchase.loc[cust_purchase['Number of active days'] <= 365]))


# In[20]:


# #3 Using the elbow method to find out the optimal number of #clusters. 

wcss = []

for k in range(1,10):
    
    km = KMeans(n_clusters = k, n_init = 10, init = 'k-means++', random_state = 0)
    #kmeans algorithm fits to the cust_purchase[['Number of active days']] dataset
    km = km.fit(cust_purchase[['Number of active days']])
    #kmeans inertia_ attribute is:  Sum of squared distances of samples #to their closest cluster center.
    wcss.append(km.inertia_)
    
# print(wcss) # wcss is a list.
# round to nearest num from a list.. https://www.adamsmith.haus/python/answers/how-to-round-all-elements-of-a-list-of-numbers-in-python
print([round(num, 1) for num in wcss])

plt.plot(range(1, 10), wcss)
plt.xlabel('Number of clusters (k)')
plt.ylabel('Within Cluster Sum of Squares')


# In[21]:


#run kmeans clustering with n_cluster = 3
km = KMeans(n_clusters = 3, n_init = 10, init = 'k-means++', random_state = 0)
km.fit(cust_purchase[['Number of active days']])

# Assign cluster for k means Labels of each point to active day Clusters to a new variable called active day Clusters
# review the cluster labels and assign to new table column active day Clusters
cust_purchase['Active day Clusters'] = km.labels_
cust_purchase

# Final locations of the centroid
# review the cluster centers
print(km.cluster_centers_)

# Find the number of iterations required to converge
print(km.n_iter_)


# In[22]:


# calculate the mean of each feature, in this example Number of Active days, for each cluster
cust_purchase.groupby('Active day Clusters').agg({'Number of active days':'mean'}).reset_index()


# In[23]:


cust_orderid = final[['customer_unique_id', 'order_id']]

cust_orderid = cust_orderid.groupby('customer_unique_id').agg({'order_id':'count'}).reset_index()

# Rename order_id to Number of Orders.
cust_orderid = cust_orderid.rename(columns={'order_id': 'Number of Orders'}) 

print("Below display the number of unique orders placed by each customer\n", cust_orderid.sort_values(by='Number of Orders', ascending=False).head(10))


# In[24]:


from sklearn.cluster import KMeans
wcss = []

for k in range(1,10):
    km = KMeans(n_clusters = k, n_init = 10, init = 'k-means++')
    km = km.fit(cust_orderid[['Number of Orders']])
    wcss.append(km.inertia_)

print([round(num, 1) for num in wcss])

plt.plot(range(1, 10), wcss)
plt.xlabel('Number of clusters (k)')
plt.ylabel('Within Cluster Sum of Squares');


# In[25]:


km = KMeans(n_clusters = 3, n_init = 10, init = 'k-means++', random_state = 0)
km.fit(cust_orderid[['Number of Orders']])

# Create a new variable and assign cluster for k means label, review the cluster labels for each customer_unique_id
cust_orderid['Order ID Clusters']= km.labels_
cust_orderid

# Final locations of the centroid
print(km.cluster_centers_)

# Find the number of iterations required to converge
print(km.n_iter_)

cust_orderid.groupby('Order ID Clusters').agg({'Number of Orders':'mean'}).reset_index()



# In[26]:


purchase_cost = final[[ 'customer_unique_id', 'price', 'freight_value']].copy()
# Select 1 to 2 columns to sum, 0 belong to customer_unique_id.. need to copy of a slice from a DataFrame above
purchase_cost['Total Cost']= purchase_cost['price'] + purchase_cost['freight_value']

purchase_cost = purchase_cost.groupby('customer_unique_id').agg({'Total Cost':'sum'}).reset_index()
purchase_cost

print("Below display how much money spent from each unique customer\n", purchase_cost.sort_values(by='Total Cost', ascending=False).head(10))


# In[27]:


wcss = []

for k in range(1,10):
    km = KMeans(n_clusters = k, n_init = 10, init = 'k-means++')
    km = km.fit(purchase_cost[['Total Cost']])
    wcss.append(km.inertia_)

print([round(num, 1) for num in wcss])

plt.plot(range(1, 10), wcss)
plt.xlabel('Number of clusters (k)')
plt.ylabel('Within Cluster Sum of Squares')


# In[28]:


km = KMeans(n_clusters = 3, n_init = 10, init = 'k-means++', random_state = 0)
km.fit(purchase_cost[['Total Cost']])

# Create a new variable and assign cluster for k means label
purchase_cost['Total Cost cluster']= km.labels_
purchase_cost

# Final locations of the centroid
print(km.cluster_centers_)

# Find the number of iterations required to converge
print(km.n_iter_)

purchase_cost.groupby('Total Cost cluster').agg({'Total Cost':'mean'}).reset_index()


# In[32]:


# create a variable called segmentation to merge cust_purchase, cust_orderid, purchase_cost dataframes together base upon on customer uuid
segmentation = cust_purchase.merge(cust_orderid, on='customer_unique_id')
segmentation = segmentation.merge(purchase_cost, on='customer_unique_id')

# visualize the distribution
fig = plt.figure(figsize=(20, 15))
plt.subplot(3, 1, 1); sns.histplot(segmentation['Number of active days'], bins=50)
plt.xlabel('Active days from last purchase')
plt.ylabel('Count')
plt.title("Distribution of Active days from last purchase");

plt.subplot(3, 1, 2); sns.histplot(segmentation['Number of Orders'], bins=25)
plt.xlabel('Number of Orders')
plt.ylabel('Count')
plt.title("Distribution of unique orders placed by each customer");

plt.subplot(3, 1, 3); sns.histplot(segmentation['Total Cost'])
plt.show()

# segmentation.loc[segmentation['Number of Orders'] == 24]
segmentation.loc[segmentation['Total Cost'] == 13664.08]


# In[33]:


segmentation['total_score'] = segmentation['Active day Clusters'] + segmentation['Order ID Clusters'] + segmentation['Total Cost cluster']

segmentation_clusters = segmentation[['Active day Clusters', 'Order ID Clusters', 'Total Cost cluster']] # dataframe we want to use for clustering

scaler = StandardScaler() # set up scaler

scaler.fit(segmentation_clusters) # fit our data to the scaler

segmentation_norm = scaler.transform(segmentation_clusters) # trasnform data with scaler
segmentation_norm = pd.DataFrame(data=segmentation_norm, index=segmentation_clusters.index, columns=segmentation_clusters.columns) # convert back to dataframe

segmentation_norm.head()

wcss = []
for k in range(1,10):
    km = KMeans(n_clusters = k, n_init = 10, init = 'k-means++')
    km = km.fit(segmentation_norm)
    wcss.append(km.inertia_)

print(wcss)

plt.plot(range(1, 10), wcss)
plt.xlabel('Number of clusters (k)')
plt.ylabel('Within Cluster Sum of Squares')
plt.title('Clusters vs the SSE')
plt.show()


# Below result shows a choosen model with 4 clusters results in an SSE of 64556.9.

# In[36]:


k_means = KMeans(n_clusters=4, n_init = 10, init = 'k-means++', random_state = 0 ) 

k_means.fit(segmentation_norm)

# Assign k means label to each cluster
cluster = k_means.labels_

segmentation_group = segmentation[['Number of active days', 'Number of Orders', 'Total Cost']]

# Assign labels to desired cluster t analyze.
segmentation_clusters_df = segmentation_group.assign(Clusters = cluster)

# Print Mean and median by associated with active days, Number of Orders, Total Cost group by clusters..
segmentation_clusters_df.groupby('Clusters').agg({
    'Number of active days': [np.mean, np.median, np.size],
    'Number of Orders': [np.mean, np.median, np.size],
    'Total Cost': [np.mean, np.median, np.size]
  })


# In[38]:


# visualize the distribution
# Once the data are fit, we can access labels from the labels_ attribute. Below, we visualize the data we just fit
fig, ax = plt.subplots(3, 1, figsize=(20,15))
sns.scatterplot(x='Number of active days', y='Number of Orders', ax=ax[0], data=segmentation_clusters_df, hue = 'Clusters').set_title('Clusters according to Orders and Active days')
sns.scatterplot(x='Number of active days', y='Total Cost', ax=ax[1], data=segmentation_clusters_df, hue = 'Clusters').set_title('Clusters according to Product Cost and Active days')
sns.scatterplot(x='Number of Orders', y='Total Cost', ax=ax[2], data=segmentation_clusters_df, hue = 'Clusters').set_title('Clusters according to Product Cost and Orders')


# In[39]:


sns.boxplot(x = cluster, y=segmentation_group['Number of Orders'])


# In[40]:


sns.boxplot(x = cluster, y=segmentation_group['Total Cost'])


# In[43]:


sns.boxplot(x = cluster, y=segmentation_group['Number of active days'])


# In[ ]:




