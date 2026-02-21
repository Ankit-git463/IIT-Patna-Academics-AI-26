# %% [markdown]
# ## Submission By Ankit Singh 
# ## 2201AI47

# %% [markdown]
# ## Importing Libraries

# %%
# Importing the libraries

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt


# %%
# Importing Metrics for Evaluation

from sklearn.metrics import confusion_matrix, classification_report, accuracy_score, roc_curve, auc, RocCurveDisplay, f1_score , accuracy_score ,precision_score , recall_score

# %%
# Importing the data 

df = pd.read_csv("heart.csv")
df.tail()

# %%
# Checking the number of rows and columns
df.shape

# %%
X = df.drop("output",axis=1)
y = df["output"]
print(X.shape)
y.shape

# %%
X.head()

# %%
y.head()

# %% [markdown]
# #### Splitting the dataSet 

# %%
from sklearn.model_selection import train_test_split
X_train_80, X_test_20, y_train_80, y_test_20 = train_test_split(X,y,test_size = 0.2,random_state=42)
X_train_70, X_test_30, y_train_70, y_test_30 = train_test_split(X,y,test_size = 0.3,random_state=42)

# %%
print(X_train_80.shape)
print(X_train_70.shape)
print(X_test_20.shape)
print(X_test_30.shape)
print(y_train_80.shape)
print(y_train_70.shape)
print(y_test_20.shape)
print(y_test_30.shape)

# %% [markdown]
# ## Scaling the data 
# 

# %%
from sklearn.preprocessing import StandardScaler
scaler = StandardScaler()

# %%
scaled_X_train_80= scaler.fit_transform(X_train_80)

# %%
scaled_X_train_70= scaler.fit_transform(X_train_70)

# %%
scaled_X_test_20 = scaler.transform(X_test_20)

# %%
scaled_X_test_30 = scaler.transform(X_test_30)

# %% [markdown]
# ## Defining Functions that are used --
# 

# %% [markdown]
# 
# ### Function to print Confusion Matrix 

# %%
## Function to print Confusion matrix 

def print_confusion_matrix(test, preds):

    print("Confusion Matrix:")
    print(confusion_matrix(test, preds))
    print("\nClassification Report:")
    print(classification_report(test, preds))

# %% [markdown]
# ### Function to draw ROC-AUC Curve

# %%
def auc_curve_printing(test, pred):

    fpr, tpr, thresholds = roc_curve(test, pred)
    roc_auc = auc(fpr, tpr)
    print("ROC AUC Score:", roc_auc)
    plt.figure()
    plt.plot(fpr, tpr, color='darkorange', lw=2, label='ROC curve (area = %0.2f)' % roc_auc)
    plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('Receiver Operating Characteristic (ROC)')
    plt.legend(loc="lower right")
    plt.show()

# %% [markdown]
# ### Function for Ablation Study

# %%
def ablation_study(classifier , X_train_80 , X_test_20 , y_train_80 , y_test_20 ):
    # Ablation Study For LDA 

    columns = [ str(feature) for feature in X_train_80.columns]
    dict ={}
    max_accuracy =0 

    for col in columns :
        ab_X_train = X_train_80[col].values.reshape((-1,1))
        ab_X_test = X_test_20[col].values.reshape((-1,1))
        
        classifier.fit(ab_X_train, y_train_80)

        y_pred = classifier.predict(ab_X_test)
         
        print(col," : ", accuracy_score(y_test_20,y_pred=y_pred))
        dict[col] = accuracy_score(y_test_20,y_pred=y_pred)

        
    print("\nColumn With highest Accuracy is " ,max(dict, key=dict.get))


# %% [markdown]
# ### Storing the Model Performance Measures in Dictionaries

# %%
## Dictionary Storing F1-score of each classifier

F1Scores={}
Recall_Of_Model={}
Accuracy_Of_Model={}
Precision_Of_Model={}


# %% [markdown]
# # Training Models

# %% [markdown]
# ## Logistic Regression

# %% [markdown]
# #### 80 - 20 Split

# %%
from sklearn.linear_model import LogisticRegression

# %%
logreg_classifier = LogisticRegression(random_state=0)
logreg_classifier.fit(scaled_X_train_80, y_train_80)

# %% [markdown]
# #### Predicting the test results

# %%
y_pred = logreg_classifier.predict(scaled_X_test_20)

# %%

print_confusion_matrix(y_test_20, y_pred) 

# Storing the results in the dictionaries made above
classifiername ="Logistic_Regression"
F1Scores[classifiername]= f1_score(y_test_20, y_pred , average='weighted')
Recall_Of_Model[classifiername]=recall_score(y_test_20, y_pred)
Accuracy_Of_Model[classifiername]=accuracy_score(y_test_20, y_pred)
Precision_Of_Model[classifiername]=precision_score(y_test_20, y_pred)


# %%
# Print the Curve
auc_curve_printing(y_test_20, y_pred)

# %% [markdown]
# #### Ablation Study for Logistic Regression

# %%
ablation_study(LogisticRegression(random_state=0)  , X_train_80 , X_test_20 , y_train_80 , y_test_20)

# %% [markdown]
# #### 70 - 30 Split 

# %%
logreg_classifier = LogisticRegression(random_state=0)
logreg_classifier.fit(scaled_X_train_70, y_train_70)

# %% [markdown]
# #### Predicting the test results

# %%
preds = (logreg_classifier.predict_proba(scaled_X_test_30)[:, 1] >= 0.6).astype(int)

# %%

print_confusion_matrix(y_test_30 , preds)

# print("Confusion Matrix:")
# print(confusion_matrix(y_test_30, preds))
# print("\nClassification Report:")
# print(classification_report(y_test_30, preds))

# %%
auc_curve_printing(y_test_30, preds)

# %%

ablation_study(LogisticRegression(random_state=0) , X_train_80 , X_test_20 , y_train_80 , y_test_20 )



# %% [markdown]
# ## KNN
# 

# %% [markdown]
# #### 80 -20 Split

# %%
from sklearn.neighbors import KNeighborsClassifier

# %%
knn_classifier = KNeighborsClassifier(n_neighbors=5)
knn_classifier.fit(scaled_X_train_80, y_train_80)

# %% [markdown]
# #### Predicting the test results

# %%
y_pred = knn_classifier.predict(scaled_X_test_20)

# %%
print_confusion_matrix(y_test_20, y_pred) 
classifiername ="KNN_Classifier"
F1Scores[classifiername]= f1_score(y_test_20, y_pred , average='weighted')
Recall_Of_Model[classifiername]=recall_score(y_test_20, y_pred)
Accuracy_Of_Model[classifiername]=accuracy_score(y_test_20, y_pred)
Precision_Of_Model[classifiername]=precision_score(y_test_20, y_pred)

# %%

auc_curve_printing(y_test_20, y_pred)

# %% [markdown]
# #### Ablation Study for KNN

# %%
ablation_study(KNeighborsClassifier(n_neighbors=5) , X_train_80 , X_test_20 , y_train_80 , y_test_20 )


# %% [markdown]
# #### 70 - 30 Split  

# %%
knn_classifier = KNeighborsClassifier(n_neighbors=5)
knn_classifier.fit(scaled_X_train_70, y_train_70)

# %% [markdown]
# ### Predicting the test results

# %%
y_pred = knn_classifier.predict(scaled_X_test_30)

# %%
print_confusion_matrix(y_test_30, y_pred) 

# %%
auc_curve_printing(y_test_30, y_pred)

# %% [markdown]
# ## LDA Classifier

# %% [markdown]
# #### 80 - 20 Split 

# %%
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis

# %%
lda_classifier = LinearDiscriminantAnalysis()
lda_classifier.fit(scaled_X_train_80, y_train_80)

# %% [markdown]
# #### Predicting the test results

# %%
y_pred = lda_classifier.predict(scaled_X_test_20)

# %%
print_confusion_matrix(y_test_20, y_pred)
classifiername ="LDA_Classifier"
F1Scores[classifiername]= f1_score(y_test_20, y_pred , average='weighted')
Recall_Of_Model[classifiername]=recall_score(y_test_20, y_pred)
Accuracy_Of_Model[classifiername]=accuracy_score(y_test_20, y_pred)
Precision_Of_Model[classifiername]=precision_score(y_test_20, y_pred)

# %%
auc_curve_printing(y_test_20, y_pred)

# %% [markdown]
# #### Ablation Study for LDA

# %%
ablation_study( LinearDiscriminantAnalysis() , X_train_80 , X_test_20 , y_train_80 , y_test_20 )


# %% [markdown]
# #### 70 - 30 Split

# %%
lda_classifier = LinearDiscriminantAnalysis()
lda_classifier.fit(scaled_X_train_70, y_train_70)

# %% [markdown]
# ### Predicting the test results

# %%
y_pred = lda_classifier.predict(scaled_X_test_30)

# %%
print_confusion_matrix(y_test_30, y_pred) 

# %%
auc_curve_printing(y_test_30, y_pred)

# %% [markdown]
# ## Naive Bayes
# 
# 

# %% [markdown]
# #### 80 - 20 Split

# %%
from sklearn.naive_bayes import GaussianNB

# %%
naive_bayes_classifier = GaussianNB()
naive_bayes_classifier.fit(scaled_X_train_80, y_train_80)

# %% [markdown]
# #### Predicting the results

# %%
y_pred = naive_bayes_classifier.predict(scaled_X_test_20)

# %%
print_confusion_matrix(y_test_20 , y_pred)
classifiername ="Naive_Bayes_Classifier"
F1Scores[classifiername]= f1_score(y_test_20, y_pred , average='weighted')
Recall_Of_Model[classifiername]=recall_score(y_test_20, y_pred)
Accuracy_Of_Model[classifiername]=accuracy_score(y_test_20, y_pred)
Precision_Of_Model[classifiername]=precision_score(y_test_20, y_pred)

# %%
auc_curve_printing(y_test_20 , y_pred)

# %% [markdown]
# #### Ablation Study for Naive Bayes

# %%
ablation_study(GaussianNB(), X_train_80 , X_test_20 , y_train_80 , y_test_20 )


# %% [markdown]
# #### 70 - 30 Split

# %%
naive_bayes_classifier = GaussianNB()
naive_bayes_classifier.fit(scaled_X_train_70, y_train_70)

# %% [markdown]
# #### Predicting the results

# %%
preds = (naive_bayes_classifier.predict_proba(scaled_X_test_30)[:, 1] >= 0.6).astype(int)

# %%
print_confusion_matrix(y_test_30 , preds)

# %%
auc_curve_printing(y_test_30 , preds)

# %% [markdown]
# ## SVM ( Support Vector Machine )

# %% [markdown]
# #### 80 - 20 Split 

# %%
from sklearn.svm import SVC

# %%
from sklearn.model_selection import GridSearchCV

# %%
svm = SVC()
param_grid = {'C':[0.01,0.1,1,10],'kernel':['linear','rbf','sigmoid']}
#dict of various parameter we are gonna test and their values to test
grid = GridSearchCV(svm,param_grid)

# %%
grid.fit(scaled_X_train_80,y_train_80)

# %%
grid.best_params_

# %%
svm_classifier = SVC(kernel='rbf',C=1, random_state=0)
svm_classifier.fit(scaled_X_train_80, y_train_80)

# %% [markdown]
# #### Predicting the results

# %%
y_pred = svm_classifier.predict(scaled_X_test_20)

# %%
print_confusion_matrix(y_test_20, y_pred)
classifiername ="SVM_Classifier"
F1Scores[classifiername]= f1_score(y_test_20, y_pred , average='weighted')
Recall_Of_Model[classifiername]=recall_score(y_test_20, y_pred)
Accuracy_Of_Model[classifiername]=accuracy_score(y_test_20, y_pred)
Precision_Of_Model[classifiername]=precision_score(y_test_20, y_pred)

# %% [markdown]
# #### Ablation Study for SVM

# %%
auc_curve_printing(y_test_20, y_pred)

# %%
ablation_study( SVC(kernel='linear',C=1, random_state=0) , X_train_80 , X_test_20 , y_train_80 , y_test_20)

# %% [markdown]
# #### 70 - 30 Split 

# %%

svm = SVC()
param_grid = {'C':[0.01,0.1,1,10],'kernel':['linear','rbf','sigmoid']}
#dict of various parameter we are gonna test and their values to test
grid = GridSearchCV(svm,param_grid)


# %%
grid.fit(scaled_X_train_70,y_train_70)

# %%
grid.best_params_

# %%
svm_classifier = SVC(kernel='linear',C=1, random_state=0)
svm_classifier.fit(scaled_X_train_70, y_train_70)

# %% [markdown]
# #### Predicting the results

# %%
df = svm_classifier.decision_function(scaled_X_test_30) 
preds =[] 
for i in df: 
    if i<0.6: 
        preds.append(0) 
    else: 
        preds.append(1)

# %%
print_confusion_matrix(y_test_30, preds)

# %%
auc_curve_printing(y_test_30, preds)

# %% [markdown]
# # Comparing the Models 

# %% [markdown]
# #### F1 Scores

# %%
print("F1-Scores of each Classifier are : \n")

for val in F1Scores:
    print(val ,"  : " ,F1Scores[val])

print ( "\nThe Classifiers with best F1 Score is\n", max(F1Scores, key=F1Scores.get))
print ("\nF1-Score of" , max(F1Scores, key=F1Scores.get) , "is :" , F1Scores[max(F1Scores, key=F1Scores.get)])

# %% [markdown]
# #### Recall

# %%
print("Recall of each Classifier are : \n")

for val in Recall_Of_Model:
    print(val ,"  : " ,Recall_Of_Model[val])

print ( "\nThe Classifiers with best Recall is\n", max(Recall_Of_Model, key=Recall_Of_Model.get))
print ("\nRecall of" , max(Recall_Of_Model, key=Recall_Of_Model.get) , "is :" , Recall_Of_Model[max(Recall_Of_Model, key=Recall_Of_Model.get)])

# %% [markdown]
# #### Precision Scores 

# %%
print("Precision of each Classifier are : \n")

for val in Precision_Of_Model:
    print(val ,"  : " ,Precision_Of_Model[val])

print ( "\nThe Classifiers with best Precision is\n", max(Precision_Of_Model, key=Precision_Of_Model.get))
print ("\nPrecision of" , max(Precision_Of_Model, key=Precision_Of_Model.get) , "is :" , Precision_Of_Model[max(Precision_Of_Model, key=Precision_Of_Model.get)])

# %% [markdown]
# #### Accuracy

# %%
print("Accuracy of each Classifier are : \n")

for val in Accuracy_Of_Model:
    print(val ,"  : " ,Accuracy_Of_Model[val])

print ( "\nThe Classifiers with best Accuracy is\n", max(Accuracy_Of_Model, key=Accuracy_Of_Model.get))
print ("\nAccuracy of" , max(Accuracy_Of_Model, key=Accuracy_Of_Model.get) , "is :" , Accuracy_Of_Model[max(Accuracy_Of_Model, key=Accuracy_Of_Model.get)])

# %% [markdown]
# ## COMPARISON BY PLOTS

# %%
import matplotlib.pyplot as plt
import pandas as pd


df_f1_scores = pd.DataFrame(F1Scores, index=['F1 Score'])
df_recall = pd.DataFrame(Recall_Of_Model, index=['Recall'])
df_accuracy = pd.DataFrame(Accuracy_Of_Model, index=['Accuracy'])
df_precision = pd.DataFrame(Precision_Of_Model, index=['Precision'])

# Plotting individual bar plots
fig, axs = plt.subplots(4, figsize=(8, 15))

# Plot for F1 Scores
df_f1_scores.plot(kind='bar', ax=axs[0], legend=True, width=0.3)
axs[0].set_title('F1 Scores')
axs[0].legend(loc='upper right')

# Plot for Recall
df_recall.plot(kind='bar', ax=axs[1], legend=True, width=0.3)
axs[1].set_title('Recall')
axs[1].legend(loc='upper right')

# Plot for Accuracy
df_accuracy.plot(kind='bar', ax=axs[2], legend=True, width=0.3)
axs[2].set_title('Accuracy')
axs[2].legend(loc='upper right')

# Plot for Precision
df_precision.plot(kind='bar', ax=axs[3], legend=True, width=0.3)
axs[3].set_title('Precision')
axs[3].legend(loc='upper right')

plt.subplots_adjust(hspace=0.5)  # Adjusting space between subplots
plt.tight_layout()

# Display the plot
plt.show()


# %% [markdown]
# ## Comaparison Table

# %%
# Create a DataFrame from dictionaries
df = pd.DataFrame([F1Scores, Recall_Of_Model, Accuracy_Of_Model, Precision_Of_Model],
                  index=['F1Scores', 'Recall', 'Accuracy', 'Precision'])

print(df)

# %%



