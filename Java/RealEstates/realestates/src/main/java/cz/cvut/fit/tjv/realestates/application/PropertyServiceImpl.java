package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import cz.cvut.fit.tjv.realestates.repository.JPACustomerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAOwnerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAPropertyRepository;
import jakarta.persistence.EntityNotFoundException;
import jakarta.transaction.Transactional;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
@Transactional
public class PropertyServiceImpl implements PropertyService {
    private final JPAPropertyRepository propertyRepository;
    private final JPAOwnerRepository ownerRepository;
    private final JPACustomerRepository customerRepository;

    public PropertyServiceImpl(JPAPropertyRepository propertyRepository,
                               JPAOwnerRepository ownerRepository, JPACustomerRepository customerRepository) {
        this.propertyRepository = propertyRepository;
        this.ownerRepository = ownerRepository;
        this.customerRepository = customerRepository;
    }

    @Override
    public List<Property> getPropertiesOwnedByBothOwners(Owner firstOwner, Owner secondOwner) throws EntityNotFoundException {
        if(ownerRepository.existsById(firstOwner.getEmail()) && ownerRepository.existsById(secondOwner.getEmail())) {
            return propertyRepository.findOwnerIntersection(firstOwner, secondOwner);
        }
        throw new EntityNotFoundException("Owner " + firstOwner.getEmail() + " or owner " + secondOwner.getEmail() + " not found");
    }

    @Override
    public Property getEntityById(Long id) throws EntityNotFoundException {
        return propertyRepository.findById(id).orElseThrow(() -> new EntityNotFoundException("Property with id:" + id + " not found."));
    }

    @Override
    public List<Property> getEntitiesByIds(List<Long> ids) throws EntityNotFoundException {
        List<Property> properties = new ArrayList<>();
        for(Long input: ids) {
            if(!propertyRepository.existsById(input)) {
                throw new EntityNotFoundException("Property id " + input + " not found.");
            }
            properties.add(getEntityById(input));
        }
        return properties;
    }

    @Override
    public List<Property> getAllEntities() {
        return propertyRepository.findAll();
    }

    @Override
    public Property createEntity(Property property) throws IllegalArgumentException {
        if(property.getPropertySize() <= 0 || property.getRentPrice() <= 0 || property.getStreetNumber() <= 0)
            throw new IllegalArgumentException("Property must have all number variables positive.");
        // Property must have an owner
        if(property.getOwners().isEmpty())
            throw new IllegalArgumentException("Property must have at least one owner.");
        for(Owner owner: property.getOwners()) {
            if(!ownerRepository.existsById(owner.getEmail())) {
                throw new EntityNotFoundException("Owner " + owner.getEmail() + " not found.");
            }
        }
        return propertyRepository.save(property);
    }

    @Override
    public Property updateEntity(Property property) throws EntityNotFoundException, IllegalArgumentException {
        if(propertyRepository.existsById(property.getId())) {
            if(property.getStreetNumber() <= 0 || property.getRentPrice() <= 0 || property.getPropertySize() <= 0)
                throw new IllegalArgumentException("Property must have all number variables positive.");
            return propertyRepository.save(property);
        }
        throw new EntityNotFoundException("Property with id:" + property.getId() + " not found.");
    }

    @Override
    public void deleteEntity(Long id) throws EntityNotFoundException {
        if(propertyRepository.existsById(id)) {
            propertyRepository.deleteById(id);
            return;
        }
        throw new EntityNotFoundException("Property with id:" + id + " not found.");
    }

    @Override
    public Customer addPropertyToCustomer(Long id, String email) throws EntityNotFoundException {
        if(customerRepository.existsById(email) && propertyRepository.existsById(id)) {
            Customer newCustomer = customerRepository.getReferenceById(email);
            propertyRepository.getReferenceById(id).setCustomer(newCustomer);
            return newCustomer;
        }
        throw new EntityNotFoundException("Customer " + email + " or property with id:" + id + " was not found.");
    }

    @Override
    public Customer removePropertyFromCustomer(Long id) throws EntityNotFoundException {
        if(propertyRepository.existsById(id)) {
            Customer oldCustomer = propertyRepository.getReferenceById(id).getCustomer();
            propertyRepository.getReferenceById(id).setCustomer(null);
            //customerRepository.getReferenceById(oldCustomer.getEmail()).getSubleases()
            //        .remove(propertyRepository.getReferenceById(id));
            return oldCustomer;
        }
        throw new EntityNotFoundException("Property with id:" + id + " not found.");
    }

    @Override
    public void addOwnerToProperty(Owner owner, Property property) throws EntityNotFoundException {
        if(ownerRepository.existsById(owner.getEmail()) && propertyRepository.existsById(property.getId())) {
            propertyRepository.getReferenceById(property.getId()).getOwners().add(owner);
            // ownerRepository.getReferenceById(owner.getEmail()).getOwnedProperties().add(property);
            return;
        }
        throw new EntityNotFoundException("Owner " + owner.getEmail() + " or property " + property.getId() + " not found");
    }

    @Override
    public void removeOwnerFromProperty(Owner owner, Property property) throws EntityNotFoundException, IllegalArgumentException {
        if(ownerRepository.existsById(owner.getEmail()) && propertyRepository.existsById(property.getId())) {
            Property currentProperty = propertyRepository.getReferenceById(property.getId());
            if(currentProperty.getOwners().contains(owner)) {
                currentProperty.getOwners().remove(owner);
                //Property must always have an owner
                if(currentProperty.getOwners().isEmpty()) {
                    Customer oldCustomer = propertyRepository.getReferenceById(property.getId()).getCustomer();
                    propertyRepository.getReferenceById(property.getId()).setCustomer(null);
                    //customerRepository.getReferenceById(oldCustomer.getEmail()).getSubleases()
                    //        .remove(propertyRepository.getReferenceById(property.getId()));
                    propertyRepository.deleteById(property.getId());
                }
                ownerRepository.getReferenceById(owner.getEmail()).getOwnedProperties().remove(property);
                return;
            }
            throw new EntityNotFoundException("Owner " + owner.getEmail() + " does not own provided property");
        }
        throw new EntityNotFoundException("Owner " + owner.getEmail() + " or property with id:" + property.getId() + " was not found.");
    }
}
