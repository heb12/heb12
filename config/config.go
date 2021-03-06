// Package config provides specific directories for Heb12 with the apppaths module
package config

import (
	"encoding/json"
	"io/ioutil"
	"os"

	gap "github.com/muesli/go-app-paths"
)

// DataDirs contains the directories in which different data is stored
var DataDirs = struct {
	Bibles string
	Gratis string
	Sword  string
}{
	"bibles",
	"bibles/gratis",
	"bibles/sword",
}

// DataFiles contains the different files in the data dir that have im
var DataFiles = struct {
	Highlights string
	Bookmarks  string
}{
	"highlights.json",
	"bookmarks.json",
}

func init() {
	InitDirs()
}

// GetScope returns the gap scope for Heb12
func GetScope() *gap.Scope {
	return gap.NewScope(gap.User, "heb12")
}

// GratisDir is a shortcut to get the directory for just the Gratis versions
func GratisDir() (string, error) {
	scope := GetScope()
	dataDirs, err := scope.DataDirs()
	if err != nil {
		return "", err
	}
	dataDir := dataDirs[0]

	return dataDir + "/" + DataDirs.Gratis, nil
}

// InitDirs makes all of the directories required for storing data and configuration
func InitDirs() error {
	scope := GetScope()
	dataDirs, err := scope.DataDirs()
	if err != nil {
		return err
	}
	dataDir := dataDirs[0]
	configDir, err := scope.ConfigPath("")
	if err != nil {
		return err
	}
	var dirs = []string{
		configDir,
		dataDir,
		dataDir + "/bibles/gratis",
		dataDir + "/bibles/sword",
	}
	for _, dir := range dirs {
		err := os.MkdirAll(string(dir), os.ModePerm)
		if err != nil && os.IsNotExist(err) {
			return err
		}
	}
	return nil
}

// Config contains all the configuration needed for all apps
type Config struct {
	GUI ConfigGUI `json:"gui"`
}

// ConfigGUI contains all the data stored in the config file for GUI apps
type ConfigGUI struct {
	// Locale is localization code currently being used
	Locale string `json:"locale"`
	// Book is the ID of the last book loaded
	Book string `json:"book"`
	// Chapter is the last chapter of the last book loaded
	Chapter int `json:"chapter"`
	// Translation is the translation code of the last translation loaded
	Translation string `json:"translation"`
	// Theme is the color theme chosen
	Theme string `json:"theme"`
	Font  Font   `json:"font"`
}

// Font contains font configuration information
type Font struct {
	Family string `json:"family"`
	Size   int    `json:"size"`
}

// WriteConfig writes config information to the config file
func WriteConfig(config Config) error {
	bytes, err := json.MarshalIndent(config, "", "    ")
	if err != nil {
		return err
	}

	scope := GetScope()
	configPath, err := scope.ConfigPath("config.json")
	if err != nil {
		return err
	}

	f, err := os.Create(configPath)
	if err != nil {
		return err
	}
	defer f.Close()
	_, err = f.Write(bytes)
	if err != nil {
		return err
	}
	return nil
}

// ReadConfig reads the config file and marshals it into a Config
func ReadConfig() (Config, error) {
	scope := GetScope()
	configPath, err := scope.ConfigPath("config.json")
	if err != nil {
		return Config{}, err
	}

	bytes, err := ioutil.ReadFile(configPath)
	if err != nil {
		return Config{}, err
	}

	var config Config
	err = json.Unmarshal(bytes, &config)
	if err != nil {
		return Config{}, err
	}

	return config, nil
}
