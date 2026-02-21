import streamlit as st
import pickle
from sklearn.feature_extraction.text import TfidfVectorizer
import nltk
import string
nltk.download('punkt')
nltk.download('stopwords') 

 # Download stopwords
from nltk.corpus import stopwords
from nltk.stem.porter import PorterStemmer

ps = PorterStemmer()


# Load pre-trained models
tfidf = pickle.load(open('vectorizer.pkl', 'rb'))
model = pickle.load(open('model.pkl', 'rb'))


def transform_text(text):
    text = text.lower()
    text = nltk.word_tokenize(text)

    y = []
    for i in text:
        if i.isalnum():
            y.append(i)

    text = y[:]
    y.clear()

    for i in text:
        if i not in stopwords.words('english') and i not in string.punctuation:
            y.append(i)

    text = y[:]
    y.clear()

    for i in text:
        y.append(ps.stem(i))

    return " ".join(y)

st.title("Email-SMS SPAM Classifier")

# Initialize session state for input and result
if "input_sms" not in st.session_state:
    st.session_state.input_sms = ""
if "result" not in st.session_state:
    st.session_state.result = None

# Input field for message
st.session_state.input_sms = st.text_area("Enter the message", st.session_state.input_sms, height=150)

# Button layout
col1, col2, col3 = st.columns([1, 1, 1])

if col1.button('Predict'):
    if st.session_state.input_sms.strip():
        # 1. Preprocess
        transform_sms = transform_text(st.session_state.input_sms)

        # 2. Vectorize
        vector_input = tfidf.transform([transform_sms])

        # 3. Predict
        st.session_state.result = model.predict(vector_input)[0]
    else:
        st.warning("Please enter a message before predicting.")

# Display results if prediction is made
if st.session_state.result is not None:
    st.subheader("Your Message:")
    st.info(st.session_state.input_sms)

    st.subheader("Prediction:")
    if st.session_state.result == 1:
        st.error("🚨 Spam Message")
    else:
        st.success("✅ Not a Spam Message")

# Clear Button
if col2.button("Clear"):
    st.session_state.input_sms = ""
    st.session_state.result = None
    st.rerun()  # Safely rerun the app

# Refresh Button (Same as Clear)
if col3.button("Refresh"):
    st.session_state.input_sms = ""
    st.session_state.result = None
    st.rerun()  # Safely rerun the app