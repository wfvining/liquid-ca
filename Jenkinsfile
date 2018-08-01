pipeline {
  agent any
  stages {
    stage('configure') {
      steps {
        sh 'cmake .'
      }
    }
    stage('compile') {
      steps {
        sh 'make'
      }
    }
    stage('test') {
      steps {
        sh 'make test'
      }
    }
  }
}