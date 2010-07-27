#ifndef ConfigurationItem_H
#define ConfigurationItem_H

#include <string>

namespace Misc {

class ConfigurationItem {
public:
	/** Configuration item type. */
	enum Type {
		/** A boolean flag. --help is an example. */
		Boolean,
		/** A string. --modules is an example. */
		String,
		/** An integer. --network-wait is an example. */
		Integer,
		/** A group of child items. --module-config is one example. */
		Group
	};
	
	/** Destructor for ConfigurationItem. */
	virtual ~ConfigurationItem();
protected:
	/** Protected constructor.
		@param name The name of this configuration item.
		@param type The type of this item.
		@note Only called by derived types.
	*/
	ConfigurationItem(const std::string &name, Type type);
private:
	/** The name of this ConfigurationItem instance. */
	std::string m_name;
	/** The type of this ConfigurationItem instance. */
	Type m_type;
	/** A short description of this instance, used to auto-generate --help. */
	std::string m_description;
public:
	/** Get method for returning @a m_name.
		@return A reference to @a m_name.
	*/
	const std::string &name() const { return m_name; }
	/** Get method for @a m_type.
		@return The value of @a m_type.
	*/
	Type type() const { return m_type; }
	/** Overloaded function to retrieve the string value of this item.
		@return The string value of this item.
		@note This function prints a warning if this is not a
			ConfigurationItemString type and returns an empty string.
	*/
	virtual const std::string &stringValue() const;
	virtual bool boolValue() const;
	virtual int intValue() const;
	virtual ConfigurationItem *childValue(std::string name) const;
	
	const std::string &description() const { return m_description; }
	void setDescription(const std::string &description) { m_description = description; }
	
	virtual void setValue(const std::string &value);
	virtual void setValue(const char *value);
	virtual void setValue(bool value);
	virtual void setValue(int value);
};

} // namespace Misc

#endif
