#include <cmath>
#include <Eigen/Dense>

#include "state_estimator.hpp"

StateEstimator::StateEstimator(int n, VectorXd x0, MatrixXd P0, MatrixXd Q, MatrixXd R, VectorXd Wm, VectorXd Wc){
    n_x = n;
    n_sigma = 2*n + 1;

    initialize(x0, P0)
}

void StateEstimator::initialize(double x0, Eigen::MatrixXd P0){
    x_corr = x0;
    P_corr = P0;

    X.resize(n_x, n_sigma);
    X_next.resize(n_x, n_sigma);
}

void StateEstimator::getSigmaPoints(double eta, Eigen::VectorXd x, Eigen::MatrixXd P){
    S = P.llt().matrixL();


    X.col(0) = x;
    for(int i = 1; i < n_x; i++){
        X.col(i) = x + eta*S.col(i-1);
        X.col(i+n_x) = x - eta*S.col(i-1);
    }
}

void StateEstimator::predict(Eigen::VectorXd u){

    for(int i = 0; i << n_sigma; i++){
        X_next.col(i) = X.col(i);
        Y_next.col(i) = getOutput(X_next.col(i), u);
    }

    x_pred = X_next*Wm.transpose();
    y_pred = Y_next*Wm.transpose();
}

void StateEstimator::getKalmanGain(){

}

void StateEstimator::correct(){

}

Eigen::VectorXd StateEstimator::getOutput(Eigen::VectorXd x, Eigen::VectorXd u){
    Eigen::VectorXd y(4); // need to allocate size dynamically

    y(1) = sqrt(pow(x(1) - u(1), 2) + pow(x(2) - u(2), 2)) + x(3);
    y(2) = sqrt(pow(x(1) - u(3), 2) + pow(x(2) - u(4), 2)) + x(3);
    y(3) = sqrt(pow(x(1) - u(5), 2) + pow(x(2) - u(6), 2)) + x(3);
    y(4) = sqrt(pow(x(1) - u(7), 2) + pow(x(2) - u(8), 2)) + x(3);

    return y;
}