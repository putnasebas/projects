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
        if (ownerRepository.existsById(firstOwner.getEmail()) && ownerRepository.existsById(secondOwner.getEmail())) {
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
        if(ids == null || ids.isEmpty()) return new ArrayList<>();
        List<Property> properties = new ArrayList<>();
        for (Long input : ids) {
            if (!propertyRepository.existsById(input)) {
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
        if (property.getPropertySize() <= 0 || property.getRentPrice() <= 0 || property.getStreetNumber() <= 0)
            throw new IllegalArgumentException("Property must have all number variables positive.");
        // Property must have an owner
        if (property.getOwners().isEmpty())
            throw new IllegalArgumentException("Property must have at least one owner.");
        for (Owner owner : property.getOwners()) {
            if (!ownerRepository.existsById(owner.getEmail())) {
                throw new EntityNotFoundException("Owner " + owner.getEmail() + " not found.");
            }
        }
        Property resultProperty = propertyRepository.save(property);
        // Link owners with property
        for (Owner owner : property.getOwners()) {
            ownerRepository.getReferenceById(owner.getEmail()).getOwnedProperties().add(resultProperty);
        }
        return resultProperty;
    }

    @Override
    public Property updateEntity(Property property) throws EntityNotFoundException, IllegalArgumentException {
        if (propertyRepository.existsById(property.getId())) {
            if (property.getStreetNumber() <= 0 || property.getRentPrice() <= 0 || property.getPropertySize() <= 0)
                throw new IllegalArgumentException("Property must have all number variables positive.");
            Property oldProperty = propertyRepository.getReferenceById(property.getId());
            Property newProperty = new Property(property.getId(), property.getPropertySize(), property.getRentPrice(),
                    property.getCity(), property.getStreet(), property.getStreetNumber(),
                    oldProperty.getCustomer(), oldProperty.getOwners());
            return propertyRepository.save(newProperty);
        }
        throw new EntityNotFoundException("Property with id:" + property.getId() + " not found.");
    }

    @Override
    public void deleteEntity(Long id) throws EntityNotFoundException {
        if (propertyRepository.existsById(id)) {
            for(Owner owner : propertyRepository.getReferenceById(id).getOwners()) {
                ownerRepository.getReferenceById(owner.getEmail()).getOwnedProperties().remove(propertyRepository.getReferenceById(id));
                ownerRepository.saveAndFlush(owner);
            }
            Customer customer = propertyRepository.getReferenceById(id).getCustomer();
            if(customer != null) {
                customerRepository.getReferenceById(customer.getEmail()).getSubleases().remove(propertyRepository.getReferenceById(id));
                customerRepository.saveAndFlush(customer);
            }
            propertyRepository.getReferenceById(id).getOwners().clear();
            propertyRepository.deleteById(id);
            return;
        }
        throw new EntityNotFoundException("Property with id:" + id + " not found.");
    }

    @Override
    public Customer addPropertyToCustomer(Long id, String email) throws EntityNotFoundException {
        if (customerRepository.existsById(email) && propertyRepository.existsById(id)) {
            Customer newCustomer = customerRepository.getReferenceById(email);
            Property property = propertyRepository.getReferenceById(id);
            property.setCustomer(newCustomer);
            newCustomer.getSubleases().add(property);
            return newCustomer;
        }
        throw new EntityNotFoundException("Customer " + email + " or property with id:" + id + " was not found.");
    }

    @Override
    public Customer removePropertyFromCustomer(Long id) throws EntityNotFoundException {
        if (propertyRepository.existsById(id)) {
            if (propertyRepository.getReferenceById(id).getCustomer() == null) {
                return new Customer();
            }
            String email = propertyRepository.getReferenceById(id).getCustomer().getEmail();
            propertyRepository.getReferenceById(id).setCustomer(null);
            customerRepository.getReferenceById(email).getSubleases()
                    .remove(propertyRepository.getReferenceById(id));
            return customerRepository.getReferenceById(email);
        }
        throw new EntityNotFoundException("Property with id:" + id + " not found.");
    }

    @Override
    public void addOwnerToProperty(Owner owner, Long propertyId) throws EntityNotFoundException {
        if (ownerRepository.existsById(owner.getEmail()) && propertyRepository.existsById(propertyId)) {
            Property property = propertyRepository.getReferenceById(propertyId);
            if(!propertyRepository.getReferenceById(property.getId()).getOwners()
                    .stream().map(Owner::getEmail).toList().contains(owner.getEmail())) {
                propertyRepository.getReferenceById(property.getId()).getOwners().add(owner);
                ownerRepository.getReferenceById(owner.getEmail()).getOwnedProperties().add(property);
            }
            return;
        }
        throw new EntityNotFoundException("Owner " + owner.getEmail() + " or property " + propertyId + " not found");
    }

    @Override
    public void removeOwnerFromProperty(Owner owner, Long propertyId) throws EntityNotFoundException, IllegalArgumentException {
        if (ownerRepository.existsById(owner.getEmail()) && propertyRepository.existsById(propertyId)) {
            Property currentProperty = propertyRepository.getReferenceById(propertyId);
            if (currentProperty.getOwners().contains(owner)) {
                currentProperty.getOwners().remove(owner);
                ownerRepository.getReferenceById(owner.getEmail()).getOwnedProperties().remove(currentProperty);
                //Property must always have an owner
                if (currentProperty.getOwners().isEmpty()) {
                    Customer oldCustomer = propertyRepository.getReferenceById(propertyId).getCustomer();
                    if(oldCustomer != null) {
                        propertyRepository.getReferenceById(propertyId).setCustomer(null);
                        customerRepository.getReferenceById(oldCustomer.getEmail()).getSubleases()
                                .remove(propertyRepository.getReferenceById(propertyId));
                    }
                    propertyRepository.deleteById(propertyId);
                }
                return;
            }
            throw new EntityNotFoundException("Owner " + owner.getEmail() + " does not own provided property");
        }
        throw new EntityNotFoundException("Owner " + owner.getEmail() + " or property with id:" + propertyId + " was not found.");
    }
}
