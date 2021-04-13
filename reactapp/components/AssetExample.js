import React, { useState } from 'react';
import { TouchableOpacity, Text, View, StyleSheet, Image, Button, Alert } from 'react-native';
//import Logo from '../assets/snack-icon.png';
const Logo = require('../assets/snack-icon.png');
import MqttClient from './mqtt-client';


const AppButton = ({ onPress, title }) => (
  <TouchableOpacity onPress={onPress} style={styles.appButtonContainer}>
    <Text style={styles.appButtonText}>{title}</Text>
  </TouchableOpacity>
);



export default function AssetExample() {

  return (
    <View style={styles.container}>
      <MqttClient />
        
      
      <Text style={styles.paragraph}>
        Local files and assets can be imported by dragging and dropping them into the editor
      </Text>
      <Image style={styles.logo} source={Logo} />
      <Button  onPress={() => Alert.alert("Hello World")} title="Hi There"/>
      <AppButton onPress={() => Alert.alert("Styled Button")} title="sheeesh" />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    alignItems: 'center',
    justifyContent: 'center',
    padding: 24,
  },
  paragraph: {
    margin: 24,
    marginTop: 0,
    fontSize: 14,
    fontWeight: 'bold',
    textAlign: 'center',
  },
  logo: {
    height: 128,
    width: 128,
  },

  appButtonContainer: {
    elevation: 8,
    backgroundColor: "red",
    borderRadius: 100,
    paddingVertical: 9,
    paddingHorizontal: 12
  },
  appButtonText: {
    fontSize: 18,
    color: "#fff",
    fontWeight: "bold",
    alignSelf: "center",
    textTransform: "uppercase"
  }


});
