import numpy as np
import math
np.set_printoptions(suppress=True)

def euclidean_distance(a, b):
    diff = a - b
    return np.sqrt(np.dot(diff, diff))

def load_data(csv_filename):
    """ 
    Returns a numpy ndarray in which each row repersents
    a wine and each column represents a measurement.
    """
    # open the .csv file
    csv_file = open(csv_filename, 'r')

    csv_file_reader = []
    for line in csv_file:
        csv_file_reader.append(line)

    # remove the first row
    csv_file_reader.pop(0)

    # initialize a ndarray
    ndarray = np.zeros((len(csv_file_reader), 11))

    # fill up the ndarray with the data from the file
    for i in range(0, len(csv_file_reader)):
        line = csv_file_reader[i].split(";")
        tmp_ndarray = np.zeros((1, 11))
        for j in range(11):
            tmp_ndarray[0, j] = line[j]
        ndarray[i] = tmp_ndarray

    return ndarray

def split_data(dataset, ratio=0.9):
    """
    Return a (train, test) tuple of numpy ndarrays. 
    The ratio parameter determines how much of the data should be used for 
    training. For example, 0.9 means that the training portion should contain
    90% of the data.
    """
    num_of_train = int(len(dataset) * ratio)
    train = dataset[:num_of_train]
    test = dataset[num_of_train:]

    return (train, test)

def compute_centroid(data):
    """
    Returns a 1D array (a vector), representing the centroid of the data
    set. 
    """
    centroid = sum(data[:,:]) / data.shape[0]
    return centroid

def experiment(ww_train, rw_train, ww_test, rw_test):
    """
    Train a model on the training data by creating a centroid for each class.
    Then test the model on the test data. Prints the number of total 
    predictions and correct predictions. Returns the accuracy. 
    """
    # create a centroid for each class (red, white) using the compute_centroid function
    ww_centroid = compute_centroid(ww_train)
    rw_centroid = compute_centroid(rw_train)

    # create a model using both centroids
    model = [ww_centroid, rw_centroid]

    # merge the test data sets
    ww_and_rw_test = np.vstack([ww_test, rw_test])

    correct = 0
    total_predictions = 0

    # test
    for t in range(len(ww_and_rw_test)):
        closest_distance = math.inf
        for m in range(len(model)):
            centroid = model[m]
            distance = euclidean_distance(centroid, ww_and_rw_test[t])
            if distance < closest_distance:
                closest_class = m
                closest_distance = distance
        # count the number of correct predictions
        if t < len(ww_test):
            if closest_class == 0:
                correct += 1
        elif t >= len(rw_test):
            if closest_class == 1:
                correct += 1
        # count the total number of predictions
        total_predictions += 1

    # calculate the  accuracy
    accuracy = correct / total_predictions

    # print the number of total predictions and the accuracy of the model
    print("Total predictions:", total_predictions)
    print("The accuracy of the model:", accuracy, "\n")

    return accuracy

def cross_validation(ww_data, rw_data, k):
    """
    Perform k-fold cross validation on the data and print the accuracy for each
    fold.
    """
    # split the data into k partitions
    ww_li = np.array_split(ww_data, k)
    rw_li = np.array_split(rw_data, k)
    ww_li = np.array(ww_li, dtype=object)
    rw_li = np.array(rw_li, dtype=object)

    # cross validation
    accuracy = 0
    for i in range(k):
        ww_test = ww_li[i]
        rw_test = rw_li[i]
        ww_train = np.empty([0, ww_data.shape[1]])
        rw_train = np.empty([0, rw_data.shape[1]])
        for j in range(k):
            if i != j:
                ww_train = np.vstack([ww_train, ww_li[j]])
                rw_train = np.vstack([rw_train, rw_li[j]])

        accuracy += (experiment(ww_train, rw_train, ww_test, rw_test))

    accuracy = accuracy / k
    
    return accuracy

if __name__ == "__main__":

    ww_data = load_data('whitewine.csv')
    rw_data = load_data('redwine.csv')
    # Uncomment the following lines for step 2:
    ww_train, ww_test = split_data(ww_data, 0.9)
    rw_train, rw_test = split_data(rw_data, 0.9)
    experiment(ww_train, rw_train, ww_test, rw_test)

    # Uncomment the following lines for step 3:
    k = 10
    acc = cross_validation(ww_data, rw_data, k)
    print("{}-fold cross-validation accuracy: {}".format(k,acc))

