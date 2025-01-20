package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import jakarta.persistence.EntityNotFoundException;

import java.util.List;

public interface PropertyService extends Service<Property, Long> {
    List<Property> getPropertiesOwnedByBothOwners(Owner firstOwner, Owner secondOwner) throws EntityNotFoundException;

    //Changing relations on the side which is mapping the relation
    Customer addPropertyToCustomer(Long id, String email) throws EntityNotFoundException;
    Customer removePropertyFromCustomer(Long id) throws EntityNotFoundException;

    void addOwnerToProperty(Owner owner, Long propertyId) throws EntityNotFoundException;
    void removeOwnerFromProperty(Owner owner, Long propertyId) throws EntityNotFoundException, IllegalArgumentException;
}
