pipeline {
  agent any
  stages {
    stage('compile') {
      steps {
        sh 'cmake . && make'
      }
    }
    stage('test') {
      steps {
        sh 'make test'
      }
    }
  }
}